# SoalShiftSISOP20_modul3_F11

## Soal 1

```
a. Menggunakan IPC-shared memory, thread, fork-exec.
b. Kalian bebas berkreasi dengan game ini asal tidak konflik dengan
   requirements yang ada. (Contoh: memberi nama trainer, memberi notifikasi
   kalau barang di shop sudah penuh, dan lain-lain).
c. Terdapat 2 code yaitu soal2_traizone.c dan soal2_pokezone.c.
```

### 1.d
**Soal :**
```
d. soal1_traizone.c mengandung fitur:
     i. Normal Mode
	    1. Cari Pokemon
		2. Pokedex
		3. Shop
	ii. Capture Mode
	    1. Tangkap Pokemon
		2. Item (Lullaby Powder)
		3. Kabur (Pokemon juga memiliki chance kabur)

```
**Jawaban :**
```c
// Input utama game. Masukan 3 digit angka (Contoh 1 1 0)
int i1, i2, i3;
scanf("%d %d %d", &i1, &i2, &i3);

// Masuk Capture Mode
if(i1 == 9 && i2 == 9 && i3 == 9 && foundPkmn == 1){
    captureMode = 1;
    isSearching = 0;
    isPokedex = 0;
    isShopping = 0;
    foundPkmn = 0;
    printf("-----Masuk Capture Mode-----\n");
    continue;
}

// Pindah Menu dalam Normal Mode
if(captureMode == 0 && i1 == 1){ 
    if(i2 == 1){
        isSearching = 1;
        isPokedex = 0;
        isShopping = 0;
        printf("-----Menu Pencarian-----\n");
        continue;
    } else if(i2 == 2){
        isSearching = 0;
        isPokedex = 1;
        isShopping = 0;
        printf("-----Menu Pokedex-----\n");
        continue;
    } else if(i2 == 3){
        isSearching = 0;
        isPokedex = 0;
        isShopping = 1;
        printf("-----Menu Shop-----\n");
        continue;
    }
}

// (Normal Mode 1)
// Thread pengatur pencarian Pokemon
if(pthread_equal(id,tid[0])){
    while(1){
	    while(foundPkmn == 1);
        while(isSearching == 0);
        sleep(10);
        if(isSearching == 0)
            continue;
        
        int cek = RNG(60);
        if(cek == 1){
            *gnrtPkmn = 1;
            foundPkmn = 1;
            printf("-----Ketemu seekor Pokemon. Pergi ke Capture Mode?-----\n");
        } else{
            printf("-----Belum ditemukan-----\n");
        }
    }
}

// (Normal Mode 2)
// Pengunaan Berry dan Pelepasan Pokemon
if(isPokedex == 1){
    // Menggunakan Berry
    if(i1 == 2 && i2 == 1 && i3 == 0){
        if(trnBrry < 1){
            printf("-----Tidak memiliki Berry-----\n");
        } else if(pkmnCount == 0){
            printf("-----Tidak ada pokemon-----\n");
        } else{
            trnBrry -= 1;
        int p;
        for(p=0; p<7; p++)
            if(pkmnSlot[p] == 1)
                pkmnAP[p] += 10;
        printf("-----Menggunakan Berry-----\n");
        }
    }

    // Melepas Pokemon
    if(i1 == 2 && i2 == 2 && i3 < 7){
        if(pkmnSlot[i3] == 1){
            printf("-----");
            printPokemonName(pkmnName[i3]);
            printf(" dilepas-----\n");
            trnDllr += pkmnDollar[i3];
            pkmnSlot[i3] = 0;
            pkmnCount -= 1;
        } else
            printf("-----Slot Kosong-----\n");
    } else if(i3 > 6)
        printf("-----Out of bound-----\n");
}

// Thread pengatur pengurangan AP Pokemon
if(pthread_equal(id,tid[1])){
    while(1){
        while(captureMode == 1);
        sleep(10);
        if(captureMode == 1)
            continue;

        int p;
        for(p=0; p<7; p++)
        {
            if(pkmnSlot[p] == 0)
                continue;
            else{
                pkmnAP[p] -= 10;
                if(pkmnAP[p] < 10){
                    int cek = RNG(90);
                    if(cek == 1)
                        pkmnSlot[p] = 0;
                    else
                        pkmnAP[p] = 50;
                }
            }
        }
    }
}

// (Normal Mode 3)
// Shopping

if(isShopping == 1){
    // Membeli Lullaby Podwer
    if(i1 == 3 && i2 == 1){
        if((trnLull + i3) > 99)
            i3 = 99 - trnLull;
        if(i3 > *shpLull)
            i3 = *shpLull;
        if(i3 < 1){
            printf("-----Mencapai batas item atau tidak dibeli atau stok kosong-----\n");
        } else if(trnDllr < (60 * i3)){
            printf("-----Uang tidak cukup-----\n");
        } else{
            trnDllr -= (60 * i3);
            trnLull += i3;
            *shpLull = *shpLull - i3;
            printf("-----Pembelian sukses-----\n");
        }
    }

    // Membeli Pokeball
    if(i1 == 3 && i2 == 2){
        if((trnPkbl + i3) > 99)
            i3 = 99 - trnPkbl;
        if(i3 > *shpPkbl)
            i3 = *shpPkbl;
        if(i3 < 1){
            printf("-----Mencapai batas item atau tidak dibeli atau stok kosong-----\n");
        } else if(trnDllr < (5 * i3)){
            printf("-----Uang tidak cukup-----\n");
        } else{
            trnDllr -= (5 * i3);
            trnPkbl += i3;
            *shpPkbl = *shpPkbl - i3;
            printf("-----Pembelian sukses-----\n");
        }
    }

    // Membeli Berry
    if(i1 == 3 && i2 == 3){
        if((trnBrry + i3) > 99)
            i3 = 99 - trnBrry;
        if(i3 > *shpBrry)
            i3 = *shpBrry;
        if(i3 < 1){
            printf("-----Mencapai batas item atau tidak dibeli atau stok kosong-----\n");
        } else if(trnDllr < (15 * i3)){
            printf("-----Uang tidak cukup-----\n");
        } else{
            trnDllr -= (15 * i3);
            trnBrry += i3;
            *shpBrry = *shpBrry - i3;
            printf("-----Pembelian sukses-----\n");
        }
    }
}

// Capture Mode
if(captureMode == 1){
    // Tangkap Pokemon
    if(i1 == 4 && i2 == 1){
        if(trnPkbl < 1){
            printf("-----Tidak memiliki Pokeball-----\n");
        } else{
            trnPkbl -= 1;
            printf("-----Melempar Pokeball-----\n");
            int cek;
            int captureRate = *wildCR;
            if(lullabyEffect == 1)
                cek = RNG(captureRate + 20);
            else
                cek = RNG(captureRate);

            if(cek == 1){
                printf("-----Sukses! Pokemon berhasil ditangap-----\n");
                if(pkmnCount > 6){
                    trnDllr += *wildDollar;
                    printf("-----Karena tidak ada ruang, Pokemon dilepas-----\n");
                } else{
                    int p;
                    for(p=0; p<7; p++){
                        if(pkmnSlot[p] == 0){
                            pkmnName[p] = *wildName;
                            pkmnAP[p] = 100;
                            pkmnDollar[p] = *wildDollar;
                            pkmnIsShiny[p] = *wildIsShiny;
                            pkmnCount++;
                            pkmnSlot[p] = 1;
                            break;
                        }
                    }
                }
			    captureMode = 0;
                isSearching = 0;
                isPokedex = 1;
                isShopping = 0;
                lullabyEffect = 0;
            } else{
                printf("-----Gagal menangkap Pokemon!-----\n");
            }
        }
    }

    // Menggunakan Lullaby Powder
    if(i1 == 4 && i2 == 2){
        if(trnLull < 1){
            printf("-----Tidak memiliki Lullaby Powder-----\n");
        } else{
            trnLull -= 1;
            lullabyEffect = 1;
            printf("-----Menggunakan Lullaby Powder-----\n");
        }
    }
	
    // Kabur
    if(i1 == 4 && i2 == 3){
        captureMode = 0;
        isSearching = 0;
        isPokedex = 1;
        isShopping = 0;
        lullabyEffect = 0;
        printf("-----Kabur dari Encounter-----\n");
    }
}

// Thread Pengatur Lullaby Powder Effect
if(pthread_equal(id,tid[2])){
    while(1){
    while(lullabyEffect == 0);
    sleep(10);
    lullabyEffect = 0;
    }
}

// Thread Pengatur Chance Pokemon Kabur
if(pthread_equal(id,tid[3])){
    while(1){
		while(captureMode == 0);
		sleep(20);
		if(captureMode == 0)
			continue;
		if(lullabyEffect == 0){
			int ER = *wildER;
			int cek = RNG(ER);
			if(cek == 1){
				captureMode = 0;
				isSearching = 0;
				isPokedex = 1;
				isShopping = 0;
			printf("-----Pokemon kabur. Kembali ke Menu Pokedex-----\n.");
			} else{
				printf("-----Pokemon tersebut meratap dengan hati-hati-----\n");
			}
		}
    }
}
```
**Penjelasan :**

Tiap berapa detik sekali akan terjadi perubahan seperti pengecekan wild Pokemon, pengurangan AP Pokemon, masa expire effect Lullaby Powder, dan chance Pokemon Kabur. Semua ini diatur oleh thread yang sesuai. Thread berjalan selamanya sampai program di-terminate secara manual.
Ada satu thread sendiri yang mengurus pemasukkan input dalam game. Input utama berupa 3 digit angka integer i1 i2 i3, masing-masing dipisahkan oleh spasi. Berikut adalah panduan pemasukan input game:
```
1 1 0    Menu Cari Pokemon
1 2 0    Menu Pokedex
1 3 0    Menu Shop
2 1 0    Menggunakan Berry. Dijalankan dari Menu Pokedex
2 2 i    Melepas pokemon i (0-6). Dijalankan dari Menu Pokedex
3 1 i    Membeli i Lullaby Powder. Dijalankan dari Menu Shop
3 2 i    Membeli i Pokeball. Dijalankan dari Menu Shop
3 3 i    Membeli i Berry. Dijalankan dari Menu Shop
9 9 9    Pergi ke Capture Mode kalau bertemu Pokemon
4 1 0    Tangkap Pokemon. Menggunakan Pokeball
4 2 0    Menggunakan Lullaby Powder
4 3 0    Kabur. Keluar ke Menu Pokedex
```

### 1.e
**Soal :**
```
e. soal1_pokezone.c mengandung fitur:
     i.   Shutdown Game
	 ii.  Jual Item
	 iii. Random Pokemon
```
**Jawaban :**
```c
// (iii) Thread Generator Pokemon
if(pthread_equal(id,tid[0])){
    while(1){
        sleep(1);
        generatePokemon();
        if(*gnrtPkmn == 1){
            *wildName = gnrtName;
            *wildCR = gnrtCR;
            *wildER = gnrtER;
            *wildDollar = gnrtDollar;
            *wildIsShiny = gnrtIsShiny;
            *gnrtPkmn = 0;
		}
    }
}

// (ii) Thread Penambahan Stok Item
if(pthread_equal(id,tid[1])){
    while(1){
        sleep(10);
        if((*shpLull) + 10 > 200)
            *shpLull = 200;
        else
            *shpLull = *shpLull + 10;

        if((*shpPkbl) + 10 > 200)
            *shpPkbl = 200;
        else
            *shpPkbl = *shpPkbl + 10;

        if((*shpBrry) + 10 > 200)
            *shpBrry = 200;
        else
            *shpBrry = *shpBrry + 10;
    }
}

// (i) Thread Terminator (Masukan 1 1 1 sebagai input)
if(pthread_equal(id,tid[2])){
	while(1){
        int i1, i2, i3;
        scanf("%d %d %d", &i1, &i2, &i3);
        if(i1 == 1 && i2 == 1 && i3 == 1){
            char pid1[1000]; char pid2[1000];
            snprintf(pid1, 1000, "%d", *traiPid);
            snprintf(pid2, 1000, "%d", pokePid);
            pid_t child = fork();
            int status;
            if(child == 0){
                char *argv[]={"kill", "-9", pid1, NULL};
                execv("/bin/kill", argv);
            } else {
			    while((wait(&status)) > 0);
				char *argv[]={"kill", "-9", pid2, NULL};
				execv("/bin/kill", argv);
            }
        }
    }
}
```
**Penjelasan :**
Sesuai perintah soal, dibuat beberapa pengaturan layar belakang dari kodingan ini, yakni pembuatan pokemon secara random (fungsi generatePokemon membuat Pokemon menurut rarity dan menghitung apakah ia Shiny atau bukan), penambahan stok barang shop tiap 10 detik, dan kemampuan shut down proses soal1_traizone dan soal1_pokezone.c.
Untuk terminate kedua proses, masukan input 1 1 1. Akan ada fork-exec yang menggunakan pid kedua proses untuk terminate keduanya.

## Soal 2



## Soal 3

**Soal :**
```
Buatlah sebuah program dari C untuk mengkategorikan file. Program ini akan
memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah
sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working
directory ketika program kategori tersebut dijalankan.

Berikut adalah perintah soal:
- Pada opsi -f, user bisa menambahkan argumen file yang bisa dikategorikan
  sebanyak yang user inginkan seperti contoh di atas.
  Pada program kategori ini, folder jpg,c,zip tidak dibuat secara manual,
  melainkan melalui program c. Semisal ada file yang tidak memiliki ekstensi,
  maka dia akan disimpan dalam folder “Unknown”.
- Program kategori ini juga menerima perintah (*).
  Artinya mengkategori seluruh file yang ada di working directory ketika
  menjalankan program C tersebut.
- Selain hal itu program C ini juga menerima opsi -d untuk melakukan kategori
  pada suatu directory. Untuk opsi -d ini, user hanya bisa menginput 1 directory
  saja, tidak seperti file yang bebas menginput file sebanyak mungkin.
  Hasilnya perintah di atas adalah mengkategorikan file di /path/to/directory dan
  hasilnya akan disimpan di working directory di mana program C tersebut
  berjalan (hasil kategori filenya bukan di /path/to/directory).
- Program ini tidak rekursif. Semisal di directory yang mau dikategorikan, atau
  menggunakan (*) terdapat folder yang berisi file, maka file dalam folder
  tersebut tidak dihiraukan, cukup file pada 1 level saja.
- Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan
  secara paralel sehingga proses kategori bisa berjalan lebih cepat. Dilarang
  juga menggunakan fork-exec dan system.
```
**Jawaban :**
```c
// Fungsi pembuat Thread untuk "kategori -f"
void* moveFile(void *arg)
{
	pthread_t id=pthread_self();
    /* Pengambilan ekstensi file */
    // Pembuatan folder dengan nama ekstensi file, jika file belum dibuat
    if(mkdir(destFolder, 0777) == -1);

    char destPath[10000];
	// Membuat absolute path tujuan, source menggunakan argumen fungsi
    snprintf(destPath, 10000, "%s/%s/%s", workingDir, destFolder, getFilename((char *)arg));
    moveFileUtil((char *)arg, destPath);

	return NULL;
}

// Fungsi pembuat Thread untuk "kategori *"
void* moveAllFile(void *arg)
{
	pthread_t id=pthread_self();
    /* Pengambilan ekstensi file */
    // Pembuatan folder dengan nama ekstensi file, jika file belum dibuat
    if(mkdir(destFolder, 0777) == -1);        

    char destPath[10000];
    char sourcePath[10000];
	// Membuat absolute path dari source dan tujuan
    snprintf(sourcePath, 10000, "%s/%s", workingDir, (char *)arg);
    snprintf(destPath, 10000, "%s/%s/%s", workingDir, destFolder, getFilename((char *)arg));
    moveFileUtil(sourcePath, destPath);

	return NULL;
}

// Fungsi pembuat Thread untuk "kategori -d"
void* moveTempFile(void *arg)
{
	pthread_t id=pthread_self();
    /* Pengambilan ekstensi file */
    // Pembuatan folder dengan nama ekstensi file, jika file belum dibuat
    if(mkdir(destFolder, 0777) == -1);

    char destPath[10000];
    char sourcePath[10000];
	// Membuat absolute path dari source dan tujuan
    snprintf(sourcePath, 10000, "%s/%s", tempWrkDir, (char *)arg);
    snprintf(destPath, 10000, "%s/%s/%s", workingDir, destFolder, getFilename((char *)arg));
    moveFileUtil(sourcePath, destPath);

	return NULL;
}

// Driver
int main(int argc, char **argb){
    // Mendapat working path program ketika dijalankan
    char buf[1000];
    workingDir = getcwd(buf, 1000);

    /* Initialisasi beberapa value seperti i, p, dan err. */

    if(!strcmp(argb[1], "-f")){
        while(argb[i] != NULL){
            err=pthread_create(&(tid[i-2]),NULL,&moveFile,(void *)argb[i]);
            /* Cek error dan increment nilai*/
        }
		// Menunggu semua thread menyelesaikan tugasnya
        for(p=0; p<(i-1); p++)
            pthread_join(tid[p],NULL);
    } else if(!strcmp(argb[1], "*")) {
        /* Initialisasi pengecekkan direktori */
        if(d){
            while((dir = readdir(d)) != NULL){
			    // Diperiksa apakah objek ada dan apakah objek berupa file atau direktori
                if (stat(dir->d_name, &myFile) < 0);
                else if (!S_ISDIR(myFile.st_mode))
                {
                    err=pthread_create(&(tid[i-2]),NULL,&moveAllFile,(void *)dir->d_name);
                    /* Cek error dan increment nilai*/
                } else;
            }
			// Menunggu semua thread menyelesaikan tugasnya
            for(p=0; p<(i-1); p++)
                pthread_join(tid[p],NULL);
        }
    } else if(!strcmp(argb[1], "-d")) {
        // Mengubah working direktori sesuai dengan argumen setelah -d
		chdir(argb[2]);
        tempWrkDir = getcwd(buf, 1000);
        /* Initialisasi pengecekkan direktori */
        if(d){
            while((dir = readdir(d)) != NULL){
			    // Diperiksa apakah objek ada dan apakah objek berupa file atau direktori
                if (stat(dir->d_name, &myFile) < 0);
                else if (!S_ISDIR(myFile.st_mode))
                {
                    err=pthread_create(&(tid[i-2]),NULL,&moveTempFile,(void *)dir->d_name);
                    /* Cek error dan increment nilai*/
                } else;
            }
			// Menunggu semua thread menyelesaikan tugasnya
            for(p=0; p<(i-1); p++)
                pthread_join(tid[p],NULL);
        }
    }
    return 0;
}

```
**Penjelasan :**
Pengerjaan soal menggunakan salah satu ddari 3 fungsi thread yang dibuat, sesuai dengan argumen yang dimasukan. Pada argumen "-f", akan dibuat thread sebanyak banyak argumen setelah "-f". Pada argumen "\*", akan dibuat thread sebanyak semua file dalam working directory. Pada argumen "-d", akan dibuat thread sebanyak semua file dalam path directory pada argumen setelah "-d". Tiap thread akan membuat folder dengan nama sesuai dengan ekstensi file yang diproses, jika folder belum ada. Karena tidak case sensitif, semua folder edngan huruf uppercase akan diubah menjadi lowercase. Setelah itu, thread memproses absolute path dari source dan tujuan. Fungsi moveFileUtil akan menyalin isi dari source ke tujuan dan remove source. Dalam kata lain, file tersebut dipindahkan ke dalam folder yang baru saja dibuat.

## Soal 4
### 4a.
```c
#define matrix1X 4
#define matrix1Y 2
#define matrix2X 2
#define matrix2Y 5

int matrix1 [matrix1X][matrix1Y] = {
  {1,2},
  {4,5},
  {1,3},
  {8,3}
};
int matrix2 [matrix2X][matrix2Y] = {
  {3,6,5,2,1},
  {2,1,4,3,2},
};
int matrixHasil [matrix1X][matrix2Y] = {
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0},
  {0,0,0,0,0}
};
```

diatas ini adalah pendefinisian ukuran matrix yang akan dihitung. Jika matrix MxN * NxP maka hasilnya MxP. Ada juga pendeklarasian matrix yang ingin dihitung.


```c
int jumlahThread = matrix1X*matrix2Y;
    pthread_t threads [jumlahThread];

    for (int i = 0; jumlahThread > i ; i++)
    {
        int* p;
        pthread_create(&threads[i], NULL, perkalianMatrix, (void*)(p));
    }

    for (int i = 0; i < jumlahThread; i++) {
      pthread_join(threads[i], NULL);
    }
```
Pertama kita deklarasikan dahulu jumlah thread yang ingin di gunakan, lalu looping untuk membuat threadnya dan menjalankan fungsi perkalianMatrix. Disini akan membuat thread sebanyak besar matrix hasil yaitu 20. Dan terakhir thread akan di join untuk menunggu thread yang lainnya selesai.

```c
int step = 0;

void* perkalianMatrix (void* args)
{
  int curThread = step;
  step++;
  int i = (int)curThread/matrix2Y;
  for (int j = 0; j < matrix1Y; j++)
  {
      matrixHasil[i][curThread%matrix2Y] += matrix1[i][j] * matrix2[j][curThread%matrix2Y];
  }
}
```
Fungsi perkalian matrix adalah untuk melakukan perkalian. Thread akan mengambil sebuah nilai, yang kemudian nilai tersebut akan menjadi perhitungan nilai mana yang akan diproses oleh thread tersebut. Misal, jika dapat nilai 4, maka thread akan memproses nilai matrixHasil[0][4]. Jika 5, maka matrixHasil[1][0].

```c
for (size_t i = 0; i < matrix1X; i++) {
      for (size_t j = 0; j < matrix2Y; j++) {
        printf("%d ", matrixHasil[i][j]);
      }
      printf("\n");
    }
```
Potongan diatas akan menampilkan hasil dari perkalian matrix.

```c
    key_t key = 1234;
    int shmid = shmget(key, sizeof(int[4][5]), 0666|IPC_CREAT);
    int (*matrix)[5];
    matrix = shmat(shmid,(void*)0,0);

    for (size_t i = 0; i < matrix1X; i++) {
      for (size_t j = 0; j < matrix2Y; j++) {
        matrix[i][j] = matrixHasil[i][j];
      }
    }
    shmdt(matrix);
```
Potongan diatas akan membuat shared memory yang disimpan dengan key 1234. Yang dishare adalah matrix 4x5 yang merupakan hasilnya. Perulangan di atas adalah untuk mengcopy nilai matrix hasil kedalam matrix yang akan di share.

### 4b.

```c
    key_t key =1234;
    int shmid = shmget(key, sizeof(int[4][5]), 0666 | IPC_CREAT);
    int (*matrix)[5];
    matrix = shmat(shmid,(void*)0,0);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 5; j++) {
        printf("%d ", matrix[i][j]);
      }
      printf("\n");
    }
```
Potongan diatas adalah untuk menampilkan matrix dari soal 4a ke layar. Dengan menggunakan shared memory, maka akan memasukkan nilai matrix ke dalam variable matrix. Lalu ada perulangan untuk print hasil ke  layar terminal.

```c
    int jumlahThread = 4;
    pthread_t threads [jumlahThread];

    for (size_t j = 0; j < 4; j++) {
      for (size_t k = 0; k < 5; k++) {
        hasil = 0;
        step = 0;
        for (int i = 0; jumlahThread > i ; i++)
        {
            int p = matrix[j][k];
            pthread_create(&threads[i], NULL, perkalianMatrix, (void*)(&p));
        }

        for (int i = 0; i < jumlahThread; i++)
        {
          pthread_join(threads[i], NULL);
        }
        printf("%llu ", hasil);
      }
      printf("\n");
    }
    shmdt(matrix);
    shmctl(shmid,IPC_RMID,NULL);
```
Lalu akan dibuatlah dihitunglah pertambahannya, menggunakan 4 thread disini. Akan melakukan looping sebanyak ukuran matrix hasil dari soal 4a dan kemudian membuat 4 thread dengan passing argumen nilai dari matrix yang ingin dihitung.
Lalu akan menjoinkan thread tersebut dan akan memprintkan hasil dari pertambahan.

Terakhir, akan mendetach dirinya dari shared memory, dan men-destroy shared memory tersebut.

```c
int step = 0;
unsigned long long  hasil = 0;
pthread_mutex_t lock;

void* perkalianMatrix (void* args)
{
  int *p;
  p = (int*) args;

  int curThread = step;
  step++;

  double mulai = ceil(*p * curThread/4) + 1, selesai = ceil(*p * (curThread+1)/4);
  int i, l = (int)selesai;
  for(i = (int)mulai; l >= i; i++)
  {
    pthread_mutex_lock(&lock);
    hasil += i;
    pthread_mutex_unlock(&lock);
  }
}
```
Potongan program diatas akan membagi bilangan menjadi 4. misal bilangan 12, maka thread 1 akan menghitung 1+2+3, thread 2 akan menghitung 4+5+6, thread 3 akan menghitung 7+8+9, thread 4 akan menghitung 10+11+12. variable mulai menandakan thread akan menghitung dari mana, dan variable selesai akan menandakan sampai mana.

Lalu hasilnya akan ditambahkan kedalam satu variable global. `pthread_mutex_lock(&lock)` akan melakukan mutual exclusion akan tidak ada tabrakan dalam penambahan hasil.

### 4c

```c
int fd1[2];

int main()
{
	if (pipe(fd1)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}

	if (fork() == 0)// ls
	{
		execLS();
	}

	if(fork() == 0)// wc -l
	{
		execWC();
	}
}
```
potongan diatas akan membuat pipe. Setelah itu akan membuat child process untuk menjalankan fungsi yang berbeda.

```c
void execLS ()
{
	dup2(fd1[1], 1);
	close(fd1[0]);
	close(fd1[1]);
	char *argv[] = {"ls", NULL};
	execv("/bin/ls", argv);
}
```
fungsi diatas akan melakukan command `ls`. Pertama akan melakukan duplikasi terhadap pipe 1 yang berguna untuk write kedalam pipe. akan mengeksekusi command ls dan dimasukkan ke pipe 1.

```c
void execWC()
{
	dup2(fd1[0], 0);
	close(fd1[0]);
	close(fd1[1]);
	char *argv[] = {"wc", "-l", NULL};
	execv("/usr/bin/wc", argv);
}
```
fungsi diatas akan melakukan command `wc -l`. Pertama akan melakukan duplikasi terhadap pipe 1 yang berguna untuk read dari pipe, hasil dari command `ls` tadi dan memprosesnya. Akan mengeksekusi command wc -l dan ditampilkan ke terminal.