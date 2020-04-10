#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread
#include <sys/ipc.h>
#include <sys/shm.h>

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

int main()
{
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

    for (size_t i = 0; i < matrix1X; i++) {
      for (size_t j = 0; j < matrix2Y; j++) {
        printf("%d ", matrixHasil[i][j]);
      }
      printf("\n");
    }

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
}
