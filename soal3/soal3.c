#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>


// Checking The File
int cFile(const char *fName){
    struct stat buffer;
    int exist = stat(fName, &buffer);
    if (exist == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

//Move content
void *mv(void *fName){
    char curDir[PATH_MAX];
    char dirName[200];
    char hdd[100];
    char hddName[100];
    char file[100];
    char extFile[100];
    int i;
    strcpy(extFile, fName);
    strcpy(hddName, fName);
    char *namaa = strrchr(hddName, '/');
    strcpy(hdd, namaa);

    if (hdd[1] == '.') {
        strcpy(dirName, "Hidden");
    }
    else if (strstr(fName, ".") != NULL) {
        strcpy(file, fName);
        strtok(file, ".");
        char *token = strtok(NULL, "");
        for (i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
        }
        strcpy(dirName, token);
    }
    else {
        strcpy(dirName, "Unknown");
    }
    int exist = cFile(extFile);
    if (exist)
        mkdir(dirName, 0777);

    if (getcwd(curDir, sizeof(curDir)) != NULL) {
        char *nama = strrchr(fName, '/');
        char namafile[200];
        strcpy(namafile, curDir);
        strcat(namafile, "/");
        strcat(namafile, dirName);
        strcat(namafile, nama);

        rename(fName, namafile);
    }
}

// List of File
void lof(char *bsPath) {
    char path[1000]; 
    struct dirent *dp;
    struct stat buffer;
    DIR *dir = opendir(bsPath);
    int n = 0;

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            strcpy(path, bsPath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            if (stat(path, &buffer) == 0 && S_ISREG(buffer.st_mode)) {
                pthread_t thread;
                int err = pthread_create(&thread, NULL, mv, (void *)path);
                pthread_join(thread, NULL);
            }

            lof(path);
        }
    }
    closedir(dir);
}

//Main Program
int main(int argc, char *argv[]) {
    char curDir[PATH_MAX];

    if (getcwd(curDir, sizeof(curDir)) != NULL)     {
        lof(curDir);
    }
}
