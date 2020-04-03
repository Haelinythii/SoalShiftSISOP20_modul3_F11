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



## Soal 4