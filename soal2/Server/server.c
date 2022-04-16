#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#define PORT 8080
 
int startServer(struct sockaddr_in address, int addrlen)
{
    int fd, opt = 1;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    if (bind(fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
 
    if (listen(fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
 
    return fd;
}
 
int validateUsername(FILE *fptr, char *username)
{
    char buffer[200];
    while (fscanf(fptr, "%s", buffer) != EOF)
    {
        char *token = strtok(buffer, ":");
        if (!strcmp(username, token))
        {
            return 1;
        }
    }
    return 0;
}
 
int validatePassword(char *password)
{
    // check for length, number, uppercase, and lowercase character
    int c_num = 0, c_upp = 0, c_low = 0;
    if (strlen(password) < 6)
        return 1;
    for (int i = 0; password[i] != '\0'; i++)
    {
        c_num = c_num || isdigit(password[i]);
        c_upp = c_upp || (isalpha(password[i]) && password[i] == toupper(password[i]));
        c_low = c_low || (isalpha(password[i]) && password[i] == tolower(password[i]));
    }
    if (!(c_num && c_upp && c_low))
        return 1;
 
    return 0;
}
 
void handleRegister(int fd, char *username)
{
    FILE *fptr;
    fptr = fopen("users.txt", "a+");
    char password[100];
    if (validateUsername(fptr, username) != 1)
    {
        send(fd, "Masukkan password kamu!", 100, 0);
        read(fd, password, 1024);
        if (validatePassword(password))
        {
            send(fd, "Password belum sesuai persyaratan!", 100, 0);
        }
        printf("uname %s\n", username);
        printf("pass %s\n", password);
        fprintf(fptr, "%s:%s\n", username, password);
        fclose(fptr);
        return;
    }
    else
    {
        send(fd, "Username sudah ada dalam database!", 100, 0);
        close(fd);
        exit;
    }
}
 
void handleLogin(int fd, char *username)
{
    char password[100];
    send(fd, "Masukkan password kamu!", 100, 0);
    read(fd, password, 1024);
    FILE *fptr = fopen("users.txt", "a+");
    char buffer[100], comp[201];
    snprintf(comp, 201, "%s:%s", username, password);
    while (fscanf(fptr, "%s", buffer) != EOF)
    {
        if (!strcmp(comp, buffer))
        {
            fclose(fptr);
            return;
        }
    }
    send(fd, "Login gagal!", 100, 0);
    fclose(fptr);
}
 
int validateProblem(char *title)
{
    FILE *tsv = fopen("problems.tsv", "r");
    char buffer[200];
    while (fgets(buffer, 150, tsv))
    {
        buffer[strcspn(buffer, "\n")] = 0;
        char *token = strtok(buffer, "\t");
        if (!strcmp(title, token))
        {
            return 1;
        }
    }
    fclose(tsv);
    return 0;
}
 
void copyFiles(char *sc, char *dest)
{
    FILE *fp1 = fopen(sc, "a+");
    FILE *fp2 = fopen(dest, "a+");
    char c;
    c = fgetc(fp1);
    while (c != EOF)
    {
        fputc(c, fp2);
        c = fgetc(fp1);
    }
    fclose(fp1);
    fclose(fp2);
}
 
void handleAddProblem(int fd, char *username)
{
    FILE *tsv = fopen("problems.tsv", "a+");
    char judul[150], f_desc[150], f_inp[150], f_out[150], c;
    send(fd, "Masukkan judul problem : ", 150, 0);
    read(fd, judul, 1024);
    if (validateProblem(judul))
    {
        send(fd, "re_inp", 150, 0);
        send(fd, "Judul sudah ada, silakan masukkan ulang", 150, 0);
        sleep(1);
        handleAddProblem(fd, username);
        return;
    }
    send(fd, "Masukkan filepath deskripsi problem : ", 150, 0);
    read(fd, f_desc, 1024);
    send(fd, "Masukkan filepath input problem : ", 150, 0);
    read(fd, f_inp, 1024);
    send(fd, "Masukkan filepath output problem : ", 150, 0);
    read(fd, f_out, 1024);
    fprintf(tsv, "%s\t%s\n", judul, username);
    fclose(tsv);
    mkdir(judul, 0777);
    chdir(judul);
    copyFiles(f_desc, "description.txt");
    copyFiles(f_inp, "input.txt");
    copyFiles(f_out, "output.txt");
}
 
void handleSeeProblem(int fd, char *username)
{
    char cwd[200];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    FILE *tsv = fopen("problems.tsv", "a+");
    char buffer[1000], c;
    c = fgetc(tsv);
    while (c != EOF)
    {
        if (c == '\t')
        {
            strcat(buffer, " by ");
        }
        else
        {
            strncat(buffer, &c, 1);
        }
        c = fgetc(tsv);
    }
    send(fd, buffer, 1000, 0);
 
}
 
void handleDownload(int fd, char *judul)
{
    char buffer[90], filename1[100] = "../../Client/", filename2[100] = "../../Client/", dirname[100] = "../../Client/";
    for (int i = 9; i < strlen(judul) + 1; i++)
    {
        buffer[i - 9] = judul[i];
    }
    chdir(buffer);
    strcat(dirname, buffer);
    strcat(filename1, buffer);
    strcat(filename1, "/description.txt");
    strcat(filename2, buffer);
    strcat(filename2, "/input.txt");
    mkdir(dirname, 0777);
    copyFiles("description.txt", filename1);
    copyFiles("input.txt", filename2);
}
 
void handleSubmit(int fd, char *judul)
{
    char cwd[200];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("Current working dir: %s\n", cwd);
    }
    printf("%s\n", judul);
    int p_f = 0, ln = 0;
    char buffer[100], filename1[100], filename2[100], c, c_sub;
    for (int i = 7; i < strlen(judul); i++)
    {
        buffer[i - 7] = judul[i];
    }
    for (int i = 0; i < strlen(buffer) - 2; i++)
    {
        if (buffer[i] == ' ')
        {
            filename1[i] = '\0';
            p_f = 1;
        }
        else if (!p_f)
        {
            filename1[i] = buffer[i];
        }
        else
        {
            filename2[ln] = buffer[i];
            ln++;
        }
    }
    printf("%s\n", filename1);
    chdir(filename1);
    FILE *f_inp = fopen("output.txt", "a+"), *f_sub = fopen(filename2, "a+");
    c = fgetc(f_inp);
    c_sub = fgetc(f_sub);
    while (c != EOF)
    {
        printf("%s\n", filename1);
        if (c != c_sub)
        {
            send(fd, "WA", 5, 0);
            return;
        }
        c_sub = fgetc(f_sub);
        c = fgetc(f_inp);
    }
    fclose(f_inp);
    fclose(f_sub);
    send(fd, "AC", 5, 0);
}
 
void *handleConnection(void *argv)
{
    int new_socket = *((int *)argv), auth = 0;
    free(argv);
    char response_buffer[1024], cmd_buffer[1024], username[100];
    if (!auth)
    {
        send(new_socket, "Please enter an option : [register/login]", 100, 0);
        read(new_socket, response_buffer, 1024);
        send(new_socket, "Masukkan username kamu!", 100, 0);
        read(new_socket, username, 1024);
        if (!strcmp(response_buffer, "register"))
        {
            handleRegister(new_socket, username);
        }
        else
        {
            handleLogin(new_socket, username);
        }
        auth = 1;
    }
    while (auth)
    {
        char cwd[200];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Current working dir: %s\n", cwd);
        }
        chdir("home/alex/soal-shift-sisop-modul-3-E04-2022/soal2/Server");
        send(new_socket, "\nBerhasil masuk. Masukkan perintah [add/see/download <judul-problem>/submit <judul-problem> <path-file-output.txt>]\n", 150, 0);
        read(new_socket, cmd_buffer, 1024);
        if (!strcmp(cmd_buffer, "add"))
        {
            handleAddProblem(new_socket, username);
        }
        else if (!strcmp(cmd_buffer, "see"))
        {
            handleSeeProblem(new_socket, username);
        }
        else if (cmd_buffer[0] == 'd')
        {
            handleDownload(new_socket, cmd_buffer);
        }
        else if (cmd_buffer[0] == 's')
        {
            handleSubmit(new_socket, cmd_buffer);
        }
        else
        {
            close(new_socket);
        }
    }
 
    return NULL;
}
 
int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int addrlen = sizeof(address), new_socket, s_count = 0;
    while (1)
    {
        if (!s_count)
        {
            int fd = startServer(address, addrlen);
            if ((new_socket = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            pthread_t t;
            int *p_client = malloc(sizeof(int));
            *p_client = new_socket;
            pthread_create(&t, NULL, &handleConnection, p_client);
            s_count++;
        }
    }
    return 0;
}
