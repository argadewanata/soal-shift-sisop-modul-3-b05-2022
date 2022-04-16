# Laporan Resmi Praktikum Modul 3 Kelompok B05 Sistem Operasi B Tahun 2022   
  
Nama Lengkap | NRP
------------- | -------------
Fahmi Muhazir        | 5025201043
Sidrotul Munawaroh   | 5025201047
Rere Arga Dewanata   | 5025201078 

## Soal 1  
Novak adalah seorang mahasiswa biasa yang terlalu sering berselancar di internet. Pada suatu saat, Ia menemukan sebuah informasi bahwa ada suatu situs yang tidak memiliki pengguna. Ia mendownload berbagai informasi yang ada dari situs tersebut dan menemukan sebuah file dengan tulisan yang tidak jelas. Setelah diperhatikan lagi, kode tersebut berformat base64. Ia lalu meminta kepada anda untuk membuat program untuk memecahkan kode-kode di dalam file yang Ia simpan di drive dengan cara decoding dengan base 64. Agar lebih cepat, Ia sarankan untuk menggunakan thread.

### Kendala Soal 1  
Masih kesulitan untuk melakukan decode base64, sehingga harus mencari referensi ke internet.  

### Screenshot Soal 1  
Isi folder "quote"
![Screenshot_2022-04-15_163732](/uploads/f7b0a69985cf5722eea40ea8b910b65b/Screenshot_2022-04-15_163732.jpeg)  
Isi folder "music"  
![Screenshot_2022-04-15_163757](/uploads/6d5cbcbeb53c2aa90e5d8640385cc582/Screenshot_2022-04-15_163757.jpeg)  
Isi folder "hasil"  
![Screenshot_2022-04-15_163809](/uploads/8d851ab99f172b6e91a7992a0b607015/Screenshot_2022-04-15_163809.jpeg)  
Hasil decode base64 pada file "music.txt"  
![Screenshot_2022-04-15_163819](/uploads/389bdcacaa7feb4409a8f98b0fee2187/Screenshot_2022-04-15_163819.jpeg)  
Hasil decode base64 pada file "quote.txt"  
![Screenshot_2022-04-15_163830](/uploads/4ab55c62eb7dccf741b2bd19da01ef5c/Screenshot_2022-04-15_163830.jpeg)  
Tampilan keseluruhan file dan folder  
![Screenshot_2022-04-15_163845](/uploads/d7ca19054ed6e990ef07e13cf1c0325d/Screenshot_2022-04-15_163845.jpeg)  
Isi dari "hasil.zip"  
![Screenshot_2022-04-15_163857](/uploads/2f718f7a8df4cc6002028928610ba4e6/Screenshot_2022-04-15_163857.jpeg)  
Isi pada folder "hasil" di "hasil.zip"  
![Screenshot_2022-04-15_163907](/uploads/b280153a841ae27c88606f244d362064/Screenshot_2022-04-15_163907.jpeg)  
Isi dari "no.txt"  
![Screenshot_2022-04-15_163920](/uploads/2d2f3cb84a2637c8804a27c3d3408cf7/Screenshot_2022-04-15_163920.jpeg)  
Password dibutuhkan untuk unzip file "hasil.zip"  
![Screenshot_2022-04-15_163935](/uploads/9608af078cfea3f5f4f8ccc519608709/Screenshot_2022-04-15_163935.jpeg)      

### 1A
**Deskripsi Soal**  
Download file music.zip dan quote.zip kemudian extract isinya ke dalam folder "quote" dan  "music".   

**Kode Program Fungsi Main**  
```
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

```  
**Penjelasan Kode Program Fungsi Main**  
Membuat thread untuk melakukan download zip, membuat directory baru, dan unzip. Thread dibuat menggunakan `pthread_create` Setiap thread tersebut setelah dibuat akan digabungkan menggunakan `pthread_join`.  

**Kode Program Fungsi download_zip**  
```
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
```
**Penjelasan Kode Program Fungsi download_zip**  
Sesuai dengan namanya, fungsi ini digunakan untuk mendownload zip dari internet. Agar dapat dilakukan secara bersamaan, digunakanlah thread. Untuk downloadnya sendiri, digunakan `wget` dengan parameter `-O` agar dapat disimpan dengan nama yang berbeda.  

**Kode Program Fungsi makedir**  
```
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
```  

**Penjelasan Kode Program Fungsi makedir**  
Sesuai dengan namanya, fungsi ini digunakan untuk membuat directory baru, dalam hal ini directory "music","quote", dan "hasil". Agar dapat dilakukan secara bersamaan, digunakanlah thread. Untuk membuat directory, digunakan perintah `mkdir` dengan parameter `-p` agar mampu membuat sekaligus parent directorynya.  

**Kode Program Fungsi unzip**    
```
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
```  

**Penjelasan Kode Program Fungsi unzip**  
Sesuai dengan namanya, fungsi ini digunakan untuk melakukan unzip pada file yang telah di-download sebelumnya. Agar dapat dilakukan secara bersamaan, digunakanlah thread. Untuk melakukan unzip, digunakan perintah `unzip` dengan menerima paramater `-o` agar mampu melakukan unzip tanpa prompt dan paramater `-d`agar mampu melakukan unzip ke directory yang berbeda.  

### 1B  
**Deskripsi Soal**  
Melakukan decode hasil extract pada folder "music" dan "quote" menggunakan Base64. Kemudian, masukkan hasil decode-nya ke dalam file text yang baru pada masing masing folder.  

**Kode Program Fungsi Main**  
```
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

```  
**Penjelasan Kode Program Fungsi Main**  
Membuat thread untuk melakukan decode. Thread dibuat menggunakan `pthread_create` Setiap thread tersebut setelah dibuat akan digabungkan menggunakan `pthread_join`.  

**Kode Program Untuk Decode Base64**
```
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
```  
**Penjelasan Kode Program Untuk Decode Base64**  
Untuk men-decode base64, kelompok kami mengambil referensi dari web ini https://fm4dd.com/programming/base64/base64_stringencode_c.shtm. Akan tetapi, untuk pembacaan setiap file-nya kami membuat fungsi sendiri, yaitu decode_base64text. Fungsi decode_base64text berguna untuk melakukan decode tiap iterasi item pada folder "quote" dan "music" dan hasilnya dimasukkan ke dalam file "music.txt" dan "quote.txt".  

### 1C
**Deskripsi Soal**  
Memindahkan  file "quote.txt" dan "music.txt" ke folder "hasil"  

**Kode Program Untuk Fungsi Main**  
```
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

```  
**Penjelasan Kode Program Fungsi Main**   
Membuat thread untuk melakukan pemindahan file. Thread dibuat menggunakan `pthread_create` Setiap thread tersebut setelah dibuat akan digabungkan menggunakan `pthread_join`.  

**Kode Program Untuk Fungsi Move**  
```
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
```  

**Penjelasan Kode Program Fungsi Move**  
Sesuai dengan namanya, fungsi ini digunakan untuk melakukan pemindahan file "music.txt" dan "quote.txt" ke dalam folder "hasil". Agar dapat dilakukan secara bersamaan, digunakanlah thread. Untuk melakukan pemindahan file, digunakan perintah `mv`.  

### 1D  

**Deskripsi Soal**  
Melakukan zip pada folder "hasil " dan memberikan password berupa "mihinomenestargadewanata".  

**Kode Program Fungsi zip_hasil**  
```
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
```  

**Penjelasan Kode Program Fungsi zip_hasil**  
Sesuai dengan namanya, fungsi ini digunakan untuk melakukan zip pada folder "hasil" menjadi "hasil.zip" yang dilindungi oleh password "mihinomenestargadewanata".Untuk melakukan zip, digunakan perintah `zip` dengan menerima parameter `-P` agar zip memiliki password dan parameter `-r` agar mampu melakukan zip pada directory.  

### 1E  

**Deskripsi Soal**  
Unzip file "hasil.zip" dan buat file "no.txt" yang berisi tulisan "No" pada saat yang bersamaan. Lalu, lakukan zip kembali pada folder "hasil" dan file "no.txt" dengan tetap memiliki password "mihinomenestargadewanata".  

**Kode Program Fungsi Main**  
```
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

```  
**Penjelasan Kode Program Fungsi Main**  
Membuat thread untuk melakukan unzip, membuat file "no.txt", dan zip kembali. Thread dibuat menggunakan `pthread_create` Setiap thread tersebut setelah dibuat akan digabungkan menggunakan `pthread_join`.  

**Kode Program Fungsi unzip_hasil**  
```
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
```  
**Penjelasan Kode Program fungsi unzip_hasil**  
Tidak hanya mampu unzip "hasil.zip", fungsi ini juga mampu untuk membuat file "no.txt" sekaligus mengisinya. Agar dapat dilakukan secara bersamaan, digunakanlah thread. Agar tidak terjadi kebingungan, folder "hasil" yang lama akan dihapus dengan perintah `rm` lalu dilakukan unzip dengan perintah `unzip`. Selain itu, juga terjadi proses pembuatan file "no.txt" sekaligus dilakukan penulisan di file tersebut.  

**Kode Program fungsi zip_hasil_again**  
```
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
```
**Penjelasan Kode Program fungsi zip_hasil_again**      
Tidak hanya mampu melakukan zip kembali, fungsi ini juga dapat menghapus file "hasil.zip" yang lama. Agar dapat dilakukan secara bersamaan, digunakanlah thread. Untuk melakukan zip, digunakan perintah `zip` dengan menerima parameter `-P` agar zip memiliki password dan parameter `-r` agar mampu melakukan zip pada directory.  Untuk melakukan penghapusan file "hasil.zip" yang lama digunakan perintah `rm`.    

## Soal 2  
test    

## Soal 3  
test  
