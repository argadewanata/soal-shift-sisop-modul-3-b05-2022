#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>

char *folder_name_music = "/home/argadewanata/SistemOperasi/Modul3/music";
char *folder_name_quote = "/home/argadewanata/SistemOperasi/Modul3/quote";
char *folder_name_hasil = "/home/argadewanata/SistemOperasi/Modul3/hasil";
char *file_name_music = "/home/argadewanata/SistemOperasi/Modul3/music/music.txt";
char *file_name_quote = "/home/argadewanata/SistemOperasi/Modul3/quote/quote.txt";
char *file_name_music2 = "/home/argadewanata/SistemOperasi/Modul3/hasil/music.txt";
char *file_name_quote2 = "/home/argadewanata/SistemOperasi/Modul3/hasil/quote.txt";
char *link_music = "https://drive.google.com/u/0/uc?id=1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1&export=download";
char *link_quote = "https://drive.google.com/u/0/uc?id=1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt&export=download";
char *zip_name_music = "music.zip";
char *zip_name_quote = "quote.zip";
char *zip_name_hasil = "hasil.zip";

char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

pthread_t download_id[2]; 
pthread_t makedir_id[3]; 
pthread_t unzip_id[2]; 
pthread_t decode_id[2]; 
pthread_t move_id[2];
pthread_t unzip2_id[3]; 
pthread_t zip_id[3]; 

void decodeblock(unsigned char in[], char *clrstr);
void b64_decode(char *b64src, char *clrdst);
void *decode_base64text(void *arg);

void *download_zip(void *arg);
void *unzip(void *arg);
void *makedir(void *arg);

void *move(void *arg);

void *zip_hasil();

void *unzip_hasil(void *arg);
void *zip_hasil_again(void *arg);

int main(void)
{
    /*
        Soal 1A:
        Download file music.zip dan quote.zip kemudian extract isinya ke dalam folder "quote" dan  "music"
    */

        // Download Zip 
    int err1;
    for (int i = 0; i < 2; i++)
    {
        err1 = pthread_create(&(download_id[i]),NULL,&download_zip,NULL);
    }

    for (int j = 0; j < 2 ; j++)
    {
        pthread_join(download_id[j], NULL);
    }

        // Make Directory 
    int err2;
    for (int i = 0; i < 3; i++)
    {
        err2 = pthread_create(&(makedir_id[i]), NULL, &makedir, NULL); 
    }
    
    for (int j = 0; j < 3; j++)
    {
        pthread_join(makedir_id[0], NULL);
    }
    
        // Unzip and Move to new directory
    int err3;
    for (int i = 0; i < 2; i++)
    {
        err3 = pthread_create(&(unzip_id[i]), NULL, &unzip, NULL); 
    }
    
    for (int j = 0; j < 2;j++ )
    {
        pthread_join(unzip_id[j], NULL);
    }
    
    /*
        Soal 1B:
        Decode hasil extract pada folder "music" dan "quote" menggunakan Base64.
        Kemudian, masukkan hasil decode-nya ke dalam file text yang baru pada masing masing folder
    */

    int err4;
    for (int i = 0; i < 2; i++)
    {
        err4 = pthread_create(&(decode_id[i]), NULL, &decode_base64text, NULL); 
    }
    
    for (int j = 0; j < 2; j++)
    {
        pthread_join(decode_id[j], NULL);
    }
    
    /*
        Soal 1C:
        Memindahkan  file "quote.txt" dan "music.txt" ke folder "hasil"
    */

    int err5;
    for (int i = 0; i < 2; i++)
    {
        err5 = pthread_create(&(move_id[i]), NULL, &move, NULL); 
    }
    
    for (int j = 0;j < 2 ; j++)
    {
        pthread_join(move_id[j], NULL);
    }
 
    /*
        Soal 1D:
        Melakukan zip pada folder "hasil " dan memberikan password berupa "minihomenestargadewanata"
    */

    zip_hasil();
    
    /*
        Soal 1E:
        Unzip file "hasil.zip" dan buat file "no.txt" yang berisi tulisan "No" secara bersamaan.
        Lalu, zip lagi dengan ketentuan yang sama seperti sebelumnya
    */
        // Unzip  and create "no.txt"   
    int err6;
    for (int i = 0; i < 3; i++)
    {
        err6 = pthread_create(&(unzip2_id[i]), NULL, &unzip_hasil, NULL); 
    }

    for (int j = 0; j < 3; j++)
    {
        pthread_join(unzip2_id[j], NULL);
    }
        // Zip again
    int err7;
    for (int i = 0; i < 2; i++)
    {
        err7 = pthread_create(&(zip_id[i]), NULL, &zip_hasil_again,NULL); 
    }

    for (int j = 0; j < 2; j++)
    {
        pthread_join(zip_id[j], NULL);
    }
        
    exit(0);
    return 0;
}

void *download_zip(void *arg)
{   
    int status;
    pthread_t id = pthread_self();
    if (pthread_equal(id, download_id[0]))
    {
        pid_t child_download_music;
        child_download_music = fork();
        if (child_download_music < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_download_music == 0)
        {
            char *argv[] = {"wget", "-O", zip_name_music, link_music,  NULL};
            execv("/bin/wget",argv);
        }
        while ((wait(&status)) > 0);
    }
    else if (pthread_equal(id, download_id[1]))
    {
        pid_t child_download_quote;
        child_download_quote = fork();
        if (child_download_quote < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_download_quote == 0)
        {
            char *argv[] = {"wget", "-O", zip_name_quote, link_quote,  NULL};
            execv("/bin/wget",argv);
        }
        while ((wait(&status)) > 0);
    }
}

void *makedir(void *arg)
{
    int status;
    pthread_t id = pthread_self();
    if (pthread_equal(id, makedir_id[0])) 
    {
        pid_t child_mkdir_music;
        child_mkdir_music = fork();
        if (child_mkdir_music < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_mkdir_music == 0)
        {
            char *argv[] = {"mkdir", "-p",  folder_name_music, NULL};
            execv("/bin/mkdir", argv);
        }
        while ((wait(&status)) > 0);
    }
    else if (pthread_equal(id, makedir_id[1]))
    {
        pid_t child_mkdir_quote;
        child_mkdir_quote = fork();
        if (child_mkdir_quote < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (child_mkdir_quote == 0)
        {
            char *argv[] = {"mkdir", "-p",  folder_name_quote, NULL};
            execv("/bin/mkdir", argv);
        }
        while ((wait(&status)) > 0);
    }
    else if (pthread_equal(id, makedir_id[2])) 
    {
        pid_t child_mkdir_hasil;
        child_mkdir_hasil = fork();
        if (child_mkdir_hasil < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_mkdir_hasil == 0)
        {
            char *argv[] = {"mkdir", "-p",  folder_name_hasil, NULL};
            execv("/bin/mkdir", argv);
        }
        while ((wait(&status)) > 0);
    }
}

void *unzip(void *arg)
{
    int status;
    pthread_t id = pthread_self();
    if (pthread_equal(id,unzip_id[0])) 
    {
        pid_t child_unzip_music;
        child_unzip_music = fork();
        if (child_unzip_music < 0)
        {
            exit(EXIT_FAILURE);
        }
        if (child_unzip_music == 0)
        {
            char *argv[] = {"unzip", "-o", zip_name_music, "-d", folder_name_music, NULL};
            execv("/bin/unzip", argv);
        }
        while ((wait(&status)) > 0);
    }
    else if (pthread_equal(id,unzip_id[1]))
    {
        pid_t child_unzip_quote;
        child_unzip_quote = fork();
        if (child_unzip_quote < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if (child_unzip_quote == 0)
        {
            char *argv[] = {"unzip", "-o", zip_name_quote, "-d", folder_name_quote, NULL};
            execv("/usr/bin/unzip", argv);
        }
        while ((wait(&status)) > 0);
    }
}

void decodeblock(unsigned char in[], char *clrstr)
{
    unsigned char out[4];
    out[0] = in[0] << 2 | in[1] >> 4;
    out[1] = in[1] << 4 | in[2] >> 2;
    out[2] = in[2] << 6 | in[3] >> 0;
    out[3] = '\0';
    strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst)
{
    int c, phase, i;
    unsigned char in[4];
    char *p;

    clrdst[0] = '\0';
    phase = 0;
    i = 0;
    while (b64src[i])
    {
        c = (int)b64src[i];
        if (c == '=')
        {
            decodeblock(in, clrdst);
            break;
        }
        p = strchr(b64, c);
        if (p)
        {
            in[phase] = p - b64;
            phase = (phase + 1) % 4;
            if (phase == 0)
            {
                decodeblock(in, clrdst);
                in[0] = in[1] = in[2] = in[3] = 0;
            }
        }
        i++;
    }
}

void *decode_base64text(void *arg)
{
    FILE *ptr, *musicDec, *quoteDec;
    char str[100];
    char temp[100];

    pthread_t id = pthread_self();
    if (pthread_equal(id, decode_id[0]))
    {
        for (int i = 1; i < 10; i++)
        {
            sprintf(temp, "/home/argadewanata/SistemOperasi/Modul3/music/m%d.txt", i);
            ptr = fopen(temp, "r");
            while (fgets(str, 100, ptr) != NULL)
            {
                char mydst[1024] = "";
                b64_decode(str, mydst);
                musicDec = fopen("/home/argadewanata/SistemOperasi/Modul3/music/music.txt", "a");
                fprintf(musicDec, "%s\n", mydst);
            }
            fclose(ptr);
            fclose(musicDec);
        }
    }
    else if (pthread_equal(id, decode_id[1]))
    {
        for (int i = 1; i < 10; i++)
        {
            sprintf(temp, "/home/argadewanata/SistemOperasi/Modul3/quote/q%d.txt", i);
            ptr = fopen(temp, "r");
            while (fgets(str, 100, ptr) != NULL)
            {
                char mydst[1024] = "";
                b64_decode(str, mydst);
                quoteDec = fopen("/home/argadewanata/SistemOperasi/Modul3/quote/quote.txt", "a");
                fprintf(quoteDec, "%s\n", mydst);
            }
            fclose(ptr);
            fclose(quoteDec);
        }
    }
}

void *move(void *arg)
{
    int status;    
    pthread_t id = pthread_self();
    if (pthread_equal(id, move_id[0]))
    {
        pid_t child_move_quote;
        child_move_quote = fork();
        if(child_move_quote < 0 )
        {
            exit(EXIT_FAILURE);
        }
        else if (child_move_quote == 0)
        {
            char *argv[] = {"mv",file_name_quote,folder_name_hasil,NULL};
            execv("/bin/mv", argv);
        }
        while ((wait(&status)) > 0);
    }
    else if (pthread_equal(id, move_id[1])) 
    {
        pid_t child_move_music;
        child_move_music = fork();
        if(child_move_music < 0 )
        {
            exit(EXIT_FAILURE);
        }
        else if (child_move_music == 0)
        {
            char *argv[] = {"mv",file_name_music,folder_name_hasil,NULL};
            execv("/bin/mv", argv);
        }
        while ((wait(&status)) > 0);
    }
}

void *zip_hasil()
{
    int status;
    chdir("/home/argadewanata/'modul 3'");
    pid_t child_zip_hasil;
    child_zip_hasil= fork();
    if (child_zip_hasil < 0)
    {
        exit(EXIT_FAILURE);
    }
    else if (child_zip_hasil == 0)
    {
        char *argv[] = {"zip", "-P", "mihinomenestargadewanata", "-r","hasil.zip","hasil",NULL};
        execv("/bin/zip", argv);
    }
    while ((wait(&status)) > 0);
}

void *unzip_hasil(void *arg)
{
    int status;
    pthread_t id = pthread_self();
    if(pthread_equal(id,unzip2_id[0]))
    {
        pid_t  child_remove_olddir_hasil;
        child_remove_olddir_hasil = fork();
        if(child_remove_olddir_hasil < 0 )
        {
            exit(EXIT_FAILURE);
        }
        else if(child_remove_olddir_hasil == 0)
        {
            char *argv[]={"rm","-r","hasil", NULL};
            execv("/bin/rm",argv);
        }
        while ((wait(&status)) > 0);
    }
    else if(pthread_equal(id,unzip2_id[1])){
        pid_t child_unzip_hasil;
        child_unzip_hasil = fork();
        if(child_unzip_hasil < 0 )
        {
            exit(EXIT_FAILURE);
        }
        else if(child_unzip_hasil == 0){
            usleep(1000);
            char *argv[] = {"unzip", "-P", "mihinomenestargadewanata", "-qq", "hasil.zip", NULL};
            execv("/bin/unzip", argv);
        }
        while ((wait(&status)) > 0);
    }
    else if(pthread_equal(id,unzip2_id[2])){
        FILE *fp1 = fopen("no.txt", "a");
        fprintf(fp1,"No");
        fclose(fp1);
    }
}

void *zip_hasil_again(void *arg)
{
    int status;
    pthread_t id = pthread_self();
    if(pthread_equal(id,zip_id[0]))
    {
        pid_t child_zip_hasil_again;
        child_zip_hasil_again = fork();
        if (child_zip_hasil_again < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if(child_zip_hasil_again == 0)
        {
            char *argv[]={"zip","-P","mihinomenestargadewanata", "-r", "hasil.zip", "hasil", "no.txt", NULL};
            execv("/bin/zip",argv);
        }
        while ((wait(&status)) > 0);
    }
    else if(pthread_equal(id,zip_id[1]))
    {
        pid_t child_remove_old_zip_hasil;
        child_remove_old_zip_hasil = fork();
        if (child_remove_old_zip_hasil < 0)
        {
            exit(EXIT_FAILURE);
        }
        else if(child_remove_old_zip_hasil == 0)
        {
            char *argv[]={"rm","-r","hasil.zip", NULL};
            execv("/bin/rm",argv);
        }
        while ((wait(&status)) > 0);
    }
}
