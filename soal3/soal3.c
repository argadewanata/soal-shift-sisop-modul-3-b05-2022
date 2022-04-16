#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>

int checkFile(const char *fname) {
    struct stat buffer;
    int ext = stat(fname, &buffer);
    if (ext == 0)
        return 1;
    else 
        return 0;
}

void *mv(void *fname) {
    char curdir[PATH_MAX];
    char dirname[200];
    char hidd[100];
    char hiddname[100];
    char file[100];
    char extfile[100];
    char *nama_ = strrchr(hiddname, '/');
    int i;
    
    strcpy(extfile, fname);
    strcpy(hiddname, fname);
    strcpy(hidd, nama_);

    if (hidd[1] == '.') {
        strcpy(dirname, "Hidden");
    }
    else if (strstr(fname, ".") != NULL) {
        strcpy(file, fname);
        strtok(file, ".");
        char *token = strtok(NULL, "");
        for (i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
        }
        strcpy(dirname, token);
    }
    else {
        strcpy(dirname, "Unknown");
    }
    int ext = checkFile(extfile);
    if (ext)
        mkdir(dirname, 0777);

    if (getcurdir(curdir, sizeof(curdir)) != NULL) {
        char *nama = strrchr(fname, '/');
        char nmfile[200];
        strcpy(nmfile, curdir);
        strcat(nmfile, "/");
        strcat(nmfile, nama);
        strcat(nmfile, dirname);
        rename(fname, nmfile);
    }
}

void lof(char *mainPath) {
    char path[1000];
    struct dirent *dirP;
    struct stat buffer;
    DIR *dir = opendir(mainPath);
    int n = 0;

    if (!dir)
        return;

    while ((dirP = readdir(dir)) != NULL) {
        if (strcmp(dirP->dname, ".") != 0 && strcmp(dirP->dname, "..") != 0) {
            strcpy(path, mainPath);
            strcat(path, "/");
            strcat(path, dirP->dname);

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

int main(int argc, char *argv[]) {
    char curdir[PATH_MAX];

    if (getcurdir(curdir, sizeof(curdir)) != NULL) {
        lof(curdir);
    }
}
