#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<wait.h>
#include<time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

pthread_t tid[10];
int *exitButton;
int trnLull; int *shpLull;
int trnPkbl; int *shpPkbl;
int trnBrry; int *shpBrry;
int trnDllr;
int *wildName; int *wildCR; int *wildER; int *wildDollar; int *wildIsShiny;
int pkmnSlot[7]; int pkmnName[7]; int pkmnAP[7]; int pkmnDollar[7]; int pkmnIsShiny[7];
int pkmnCount; int captureMode; int *gnrtPkmn; int *traiPid;
int isSearching; int isPokedex; int isShopping; int lullabyEffect; int foundPkmn;

int RNG(int chance);
void printPokemonName(int i);
void printInventory();
void printShop();
void printInputGuide();

void initTrainer(){
    int i;
    for(i=0; i<7; i++){
        pkmnSlot[i] = 0;
        pkmnName[i] = 0;
        pkmnAP[i] = 0;
        pkmnDollar[i] = 0;
        pkmnIsShiny[i] = 0;
    }
    pkmnCount = 0;
    captureMode = 0; isSearching = 0; isPokedex = 1; isShopping = 0;
    lullabyEffect = 0; foundPkmn = 0;
    trnLull = 0; trnPkbl = 5; trnBrry = 0; trnDllr = 3000;
}

void* trainerManager(void *arg){
    unsigned long int i=0;
    pthread_t id = pthread_self();

    // Pencari Pokemon
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

    // AP Manager
    if(pthread_equal(id,tid[1])){
        while(1){
            while(captureMode == 1);
            sleep(10);
            if(captureMode == 1)
                continue;
            //printf("---Pokemon mu merasa bosan---\n");
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

    // Lullaby Powder Effect Manager
    if(pthread_equal(id,tid[2])){
        while(1){
            while(lullabyEffect == 0);
            sleep(10);
            lullabyEffect = 0;
        }
    }

    // Pokemon Escape Manager
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

    // Main Input Manager
    if(pthread_equal(id,tid[4])){
        while(1){
            if(isPokedex == 1){
                printInventory();
            }
            if(isShopping == 1){
                printShop();
            }
            if(captureMode == 1){
                printf("-----Pokemon: ");
                printPokemonName(*wildName);
                if(*wildIsShiny)
                    printf(" [SHINY]-----\n");
                else
                    printf("-----\n");
            }
                
            int i1, i2, i3;
            scanf("%d %d %d", &i1, &i2, &i3);

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
        }
    }
}

int main(){
    int i, p;
    int err;

    initTrainer();

    key_t key;
    key = 1000; int shmid1 = shmget(key, sizeof(int), IPC_CREAT | 0666); exitButton = shmat(shmid1, NULL, 0);
    key = 1004; int shmid2 = shmget(key, sizeof(int), IPC_CREAT | 0666); shpLull = shmat(shmid2, NULL, 0);
    key = 1008; int shmid3 = shmget(key, sizeof(int), IPC_CREAT | 0666); shpPkbl = shmat(shmid3, NULL, 0);
    key = 1012; int shmid4 = shmget(key, sizeof(int), IPC_CREAT | 0666); shpBrry = shmat(shmid4, NULL, 0);
    key = 1016; int shmid5 = shmget(key, sizeof(int), IPC_CREAT | 0666); wildName = shmat(shmid5, NULL, 0);
    key = 1020; int shmid6 = shmget(key, sizeof(int), IPC_CREAT | 0666); wildCR = shmat(shmid6, NULL, 0);
    key = 1024; int shmid7 = shmget(key, sizeof(int), IPC_CREAT | 0666); wildER = shmat(shmid7, NULL, 0);
    key = 1028; int shmid8 = shmget(key, sizeof(int), IPC_CREAT | 0666); wildDollar = shmat(shmid8, NULL, 0);
    key = 1032; int shmid9 = shmget(key, sizeof(int), IPC_CREAT | 0666); wildIsShiny = shmat(shmid9, NULL, 0);
    key = 1036; int shmid10 = shmget(key, sizeof(int), IPC_CREAT | 0666); gnrtPkmn = shmat(shmid10, NULL, 0);
    key = 1040; int shmid11 = shmget(key, sizeof(int), IPC_CREAT | 0666); traiPid = shmat(shmid11, NULL, 0);

    *exitButton = 1;
    *gnrtPkmn = 0;
    *traiPid = getpid();

    printf("%d\n", *traiPid);
    printInputGuide();

    i=0;
    while(i<5){
        err=pthread_create(&(tid[i]), NULL, &trainerManager, NULL);
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    pthread_join(tid[4], NULL);

    shmdt(exitButton); shmctl(shmid1, IPC_RMID, NULL);
    shmdt(shpLull); shmctl(shmid2, IPC_RMID, NULL);
    shmdt(shpPkbl); shmctl(shmid3, IPC_RMID, NULL);
    shmdt(shpBrry); shmctl(shmid4, IPC_RMID, NULL);
    shmdt(wildName); shmctl(shmid5, IPC_RMID, NULL);
    shmdt(wildCR); shmctl(shmid6, IPC_RMID, NULL);
    shmdt(wildER); shmctl(shmid7, IPC_RMID, NULL);
    shmdt(wildDollar); shmctl(shmid8, IPC_RMID, NULL);
    shmdt(wildIsShiny); shmctl(shmid9, IPC_RMID, NULL);
    shmdt(gnrtPkmn); shmctl(shmid10, IPC_RMID, NULL);
    shmdt(traiPid); shmctl(shmid11, IPC_RMID, NULL);

    return 0;
}

int RNG(int chance){
    srand(time(NULL));
    int temp = rand()%100;
    if(temp < chance)
        return 1;
    else
        return 0;
}

void printPokemonName(int i){
    switch(i){
        case 1: printf("Bulbasaur");
                break;
        case 2: printf("Charmander");
                break;
        case 3: printf("Squirtle");
                break;
        case 4: printf("Rattata");
                break;
        case 5: printf("Caterpie");
                break;
        case 6: printf("Pikachu");
                break;
        case 7: printf("Eevee");
                break;
        case 8: printf("Jigglypuff");
                break;
        case 9: printf("Snorlax");
                break;
        case 10: printf("Dragonite");
                break;
        case 11: printf("Mew");
                break;
        case 12: printf("Mewtwo");
                break;
        case 13: printf("Moltres");
                break;
        case 14: printf("Zapdos");
                break;
        case 15: printf("Articuno");
                break;
    }
}

void printInventory(){
    int i;
    for(i=0; i<7; i++){
        if(pkmnSlot[i] == 1){
            printPokemonName(pkmnName[i]);
            printf("\tAP: %d", pkmnAP[i]);
            if(pkmnIsShiny[i] == 1)
                printf("\t[SHINY]\n");
            else
                printf("\n");
        } else
            printf("Slot is empty\n");
    }
    printf("Lullaby Powder: %d\n", trnLull);
    printf("Pokeball: %d\n", trnPkbl);
    printf("Berry: %d\n", trnBrry);
    printf("Poke Dollar: %d\n", trnDllr);
}

void printShop(){
    printf("List Item:\n");
    printf("Lullaby Powder\t60 Poke Dollar\n");
    printf("Pokeball\t5 Poke Dollar\n");
    printf("Berry\t15 Poke Dollar\n");
    printf("\nInventory:\n");
    printf("Lullaby Powder: %d\n", trnLull);
    printf("Pokeball: %d\n", trnPkbl);
    printf("Berry: %d\n", trnBrry);
    printf("Poke Dollar: %d\n", trnDllr);
}

void printInputGuide(){
    printf("A B C\n");
    printf("1 1 0\tMenu Cari Pokemon\n");
    printf("1 2 0\tMenu Pokedex\n");
    printf("1 3 0\tMenu Shop\n");
    printf("2 1 0\tMenggunakan Berry. Dijalankan dari Menu Pokedex\n");
    printf("2 2 i\tMelepas pokemon i (0-6). Dijalankan dari Menu Pokedex\n");
    printf("3 1 i\tMembeli i Lullaby Powder. Dijalankan dari Menu Shop\n");
    printf("3 2 i\tMembeli i Pokeball. Dijalankan dari Menu Shop\n");
    printf("3 3 i\tMembeli i Berry. Dijalankan dari Menu Shop\n");
    printf("9 9 9\tPergi ke Capture Mode kalau bertemu Pokemon\n");
    printf("4 1 0\tTangkap Pokemon. Menggunakan Pokeball\n");
    printf("4 2 0\tMenggunakan Lullaby Powder\n");
    printf("4 3 0\tKabur. Keluar ke Menu Pokedex.\n");
}