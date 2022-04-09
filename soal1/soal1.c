#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

char *folder_name_music = "/home/argadewanata/modul3/music";
char *folder_name_quote = "/home/argadewanata/modul3/quote";
char *folder_name_hasil = "/home/argadewanata/modul3/hasil";
char *link_music = "https://drive.google.com/u/0/uc?id=1_djk0z-cx8bgISFsMiUaXzty2cT4VZp1&export=download";
char *link_quote = "https://drive.google.com/u/0/uc?id=1jR67_JAyozZPs2oYtYqEZxeBLrs-k3dt&export=download";
char *zip_name_music = "music.zip";
char *zip_name_quote = "quote.zip";
char *zip_name_hasil = "hasil.zip";

// Thread for download and unzip
pthread_t tid[3];
// Thread for decode base64
pthread_t did[2];

void *download_mkdir_unzip(void  *arg);
void *decode_base64(void *arg);

int main ()
{
    /*
        Soal 1A:
        Download file music.zip dan quote.zip kemudian extract isinya ke dalam folder "quote" dan  "music"
    */
    int err;

    for (int i = 0; i < 3; i++)
    {
        err = pthread_create(&(tid[i]),NULL,&download_mkdir_unzip,NULL);
        if (err != 0)
        {
            printf("\n Cant create thread for download and unzip \n");
        }
    }

    for (int j = 0; j < 3 ; j++)
    {
        pthread_join(tid[j], NULL);
    }

    sleep(3);

    /*
        Soal 1B:
        Decode hasil extract pada folder "music" dan "quote" menggunakan Base64.
        Kemudian, masukkan hasil decode-nya ke dalam file text yang baru pada masing masing folder
    */

    int err2;

    for (int i = 0; i < 2;i++)
    {
        err2 = pthread_create(&(did[i]),NULL,&decode_base64,NULL);
        if (err2 != 0)
        {
            printf("\n Cant create thread for decode \n");
        }
    }

    for (int j = 0; j < 2 ; j++)
    {
        pthread_join(did[j], NULL);
    }

    sleep(3);
    

    return 0;
}

void  *download_mkdir_unzip(void  *arg)
{
    pthread_t id= pthread_self();
    if(pthread_equal(id,tid[0]))
    {
        pid_t child_music;
        child_music= fork();
        if (child_music< 0)
        {
            exit(EXIT_FAILURE);
        }
        else
        {
            int status;
            pid_t child_music_download_unzip;
            child_music_download_unzip = fork ();
            if (child_music_download_unzip < 0)
            {
                exit(EXIT_FAILURE);
            }
            else if (child_music_download_unzip == 0)
            {
                char *argv[] = {"wget", "-O", zip_name_music, link_music,  NULL};
                execv("/bin/wget", argv);
            }
            else 
            {
                while((wait(&status)) > 0);
                char *argv[] = {"unzip", "-o", zip_name_music, "-d", folder_name_music, NULL};
                execv("/bin/unzip", argv);
            }
        }
    }
    else if (pthread_equal(id,tid[1]))
    {
        pid_t child_quote;
        child_quote = fork();
        if (child_quote < 0)
        {
            exit(EXIT_FAILURE);
        }
        else 
        {
            int status;
            pid_t child_quote_download_unzip;
            child_quote_download_unzip = fork();
            if (child_quote_download_unzip < 0)
            {
                exit(EXIT_FAILURE);
            }
            else if (child_quote_download_unzip == 0)
            {
                char *argv[] = {"wget", "-O", zip_name_quote, link_quote,  NULL};
                execv("/bin/wget", argv);
            }
            else
            {
                while((wait(&status)) > 0);
                char *argv[] = {"unzip", "-o", zip_name_quote, "-d", folder_name_quote, NULL};
                execv("/bin/unzip", argv);
            }
        }
    }
    else if (pthread_equal(id,tid[2]))
    {
        pid_t child_mkdir;
        child_mkdir = fork();
        if (child_mkdir < 0)
        {
            exit(EXIT_FAILURE);
        }
        else 
        {
            int status_1;
            pid_t child_mkdir_music;
            child_mkdir_music= fork();
            if(child_mkdir_music < 0)
            {
                exit(EXIT_FAILURE);
            }
            else if (child_mkdir_music == 0 )
            {
                char *argv[] = {"mkdir", "-p",  folder_name_music, NULL};
                execv("/bin/mkdir", argv);
            }
            else
            {
                while((wait(&status_1)) > 0);

                int status_2;
                pid_t child_mkdir_quote_hasil;
                child_mkdir_quote_hasil = fork();
                if (child_mkdir_quote_hasil < 0)
                {
                    exit(EXIT_FAILURE);
                }
                else if (child_mkdir_quote_hasil == 0)
                {
                    char *argv[] = {"mkdir", "-p",  folder_name_quote, NULL};
                    execv("/bin/mkdir", argv);
                }
                else 
                {
                    while ((wait(&status_2)));
                }
            }
        }
    }
}

void *decode_base64(void *arg)
{


}