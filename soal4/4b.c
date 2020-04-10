#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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

int main()
{
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
    printf("\n");


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
}
