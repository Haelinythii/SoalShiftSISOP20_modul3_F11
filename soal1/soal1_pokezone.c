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
int *shpLull; int *shpPkbl; int *shpBrry;
int *wildName; int *wildCR; int *wildER; int *wildDollar; int *wildIsShiny;
int gnrtName; int gnrtCR; int gnrtER; int gnrtDollar; int gnrtIsShiny;
int *gnrtPkmn; int *traiPid; int pokePid;

void generatePokemon();

void* gameManager(void *arg){
    unsigned long int i=0;
    pthread_t id = pthread_self();
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
}

int main(){
    int i, p;
    int err;

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

    *exitButton = 0;
    *shpLull = 100;
    *shpPkbl = 100;
    *shpBrry = 100;
    *gnrtPkmn = 1;
    pokePid = getpid();

    while(*exitButton == 0);
    printf("Input 1 1 1 untuk mematikan permainan\n");
    i=0;
    while(i<3){
        err=pthread_create(&(tid[i]), NULL, &gameManager, NULL);
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);

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

void generatePokemon(){
    srand(time(NULL));
    int pokemonType = rand()%100;
    int pokemonName = rand()%5 + 1;
    
    if(pokemonType < 5){
        gnrtName = pokemonName + 10;
        gnrtCR = 30;
        gnrtER = 20;
        gnrtDollar = 200;
    } else if(pokemonType < 15){
        gnrtName = pokemonName + 5;
        gnrtCR = 50;
        gnrtER = 10;
        gnrtDollar = 100;
    } else{
        gnrtName = pokemonName;
        gnrtCR = 70;
        gnrtER = 5;
        gnrtDollar = 80;
    }

    int pokemonShiny = rand()%8000;
    if(pokemonShiny == 0){
        gnrtCR = *wildCR - 20;
        gnrtER = *wildER + 5;
        gnrtDollar = *wildDollar + 5000;
        gnrtIsShiny = 1;
    } else
        gnrtIsShiny = 0;
}