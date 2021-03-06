#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <termios.h>
#define PORT 8080
#define MAXROOM 10

pthread_mutex_t lock;
int players = 0, playersConnected = 0;
int socketIDs[2];
int healthPlayer1 = 100, healthPlayer2 = 100;
bool room[MAXROOM] = {false, false, false ,false, false, false, false, false, false, false}; //false kalo room kosong
int roomPlayersConnected[MAXROOM] = {0};
int socketIDsPerRoom[MAXROOM][2] = {0};
int healthPlayer1PerRoom[MAXROOM] = {100};
int healthPlayer2PerRoom[MAXROOM] = {100};


void* screen1Handler (void* args);

struct identitas
{
  char usrname[100];
  char password[100];
};

int cariRoom()
{
  for (int i = 0; i < MAXROOM; i++) {
    if(room[i] == false)
    {
      // printf("room : %d\n", i);
      return i;
    }
  }
}

void screen2Handler(int new_socket)
{
  healthPlayer1 = 100;
  healthPlayer2 = 100;

  int valread;
  int roomNow;
  char findOrLogout[100] = "";
  valread = read( new_socket , findOrLogout, 1024);

  if(strcmp(findOrLogout, "find") == 0)
  {
    roomNow = cariRoom();

    //set health untuk player
    healthPlayer1PerRoom[roomNow] = 100;
    healthPlayer2PerRoom[roomNow] = 100;

    socketIDsPerRoom[roomNow][roomPlayersConnected[roomNow]] = new_socket;

    roomPlayersConnected[roomNow]++;
    while (roomPlayersConnected[roomNow] < 2)
    {
      //wait
      char a[100] = "Waiting for player ...";
      send(new_socket , a , strlen(a) , 0 );
      usleep(100000);
    }
    char a[100] = "mulai";
    send(new_socket , a , strlen(a) , 0 );

    room[roomNow] = true;

    char menang[100] = "Game berakhir kamu menang";
    while(true)
    {
      char hit[100], hitSend[100] = "hit";
      valread = read( new_socket , hit, 1024);
      if(healthPlayer2PerRoom[roomNow] <= 0 || healthPlayer1PerRoom[roomNow] <= 0)
        break;

      if(new_socket == socketIDsPerRoom[roomNow][0])
      {
        healthPlayer2PerRoom[roomNow] -=10;
        if(healthPlayer2PerRoom[roomNow] <= 0)
        {
          strcpy(hitSend, "Game berakhir kamu kalah");
          send(socketIDsPerRoom[roomNow][1], hitSend , strlen(hitSend) , 0 );
          send(socketIDsPerRoom[roomNow][0] , menang , strlen(menang) , 0 );
          break;
        }
        //send to 1
        //printf("hit\n");
        send(socketIDsPerRoom[roomNow][1] , hitSend , strlen(hitSend) , 0 );
      }
      else if (new_socket == socketIDsPerRoom[roomNow][1])
      {
        healthPlayer1PerRoom[roomNow] -=10;
        if(healthPlayer1PerRoom[roomNow] <= 0)
        {
          strcpy(hitSend, "Game berakhir kamu kalah");
          send(socketIDsPerRoom[roomNow][0] , hitSend , strlen(hitSend) , 0 );
          send(socketIDsPerRoom[roomNow][1] , menang , strlen(menang) , 0 );
          break;
        }
        //send to 0
        //printf("hit\n");
        send(socketIDsPerRoom[roomNow][0] , hitSend , strlen(hitSend) , 0 );
      }
    }
  }

  // if(strcmp(findOrLogout, "find") == 0)
  // {
  //   playersConnected++;
  //   while(playersConnected < 2)
  //   {
  //     //wait
  //     char a[100] = "Waiting for player ...";
  //     send(new_socket , a , strlen(a) , 0 );
  //     usleep(100000);
  //   }
  //   char a[100] = "mulai";
  //   send(new_socket , a , strlen(a) , 0 );
  //
  //   char menang[100] = "Game berakhir kamu menang";
  //   while(true)
  //   {
  //     char hit[100], hitSend[100] = "hit";
  //     valread = read( new_socket , hit, 1024);
  //     if(healthPlayer2 <= 0 || healthPlayer1 <= 0)
  //       break;
  //     if(new_socket == socketIDs[0])
  //     {
  //       healthPlayer2 -=10;
  //       if(healthPlayer2 <= 0)
  //       {
  //         strcpy(hitSend, "Game berakhir kamu kalah");
  //         send(socketIDs[1] , hitSend , strlen(hitSend) , 0 );
  //         send(socketIDs[0] , menang , strlen(menang) , 0 );
  //         break;
  //       }
  //       //send to 1
  //       printf("hit\n");
  //       send(socketIDs[1] , hitSend , strlen(hitSend) , 0 );
  //     }
  //     else if (new_socket == socketIDs[1])
  //     {
  //       healthPlayer1 -=10;
  //       if(healthPlayer1 <= 0)
  //       {
  //         strcpy(hitSend, "Game berakhir kamu kalah");
  //         send(socketIDs[0] , hitSend , strlen(hitSend) , 0 );
  //         send(socketIDs[1] , menang , strlen(menang) , 0 );
  //         break;
  //       }
  //       //send to 0
  //       //printf("hit\n");
  //       send(socketIDs[0] , hitSend , strlen(hitSend) , 0 );
  //     }
  //   }
  // }
  else if(strcmp(findOrLogout, "logout") == 0)
  {
    screen1Handler(&new_socket);
  }
  else
  {
    screen2Handler(new_socket);
    return;
  }
  sleep(1);
  playersConnected = 0;

  roomPlayersConnected[roomNow] = 0; //reset socketID, playersConnected
  socketIDsPerRoom[roomNow][0] = 0;
  socketIDsPerRoom[roomNow][1] = 0;
  room[roomNow] = false;

  screen2Handler(new_socket);
}

void* screen1Handler (void* args)
{
  int valread, new_socket = *(int*)args;
  char msgServer [100] = "", loginRegister[100] = "", usrname[100] = "", pass[100] = "";
  struct identitas id;
  valread = read( new_socket , loginRegister, 1024);
  if(!strcmp(loginRegister, "login") == 0 && !strcmp(loginRegister, "register") == 0)
  {
    screen1Handler(&new_socket);
    pthread_cancel(pthread_self());
  }
  valread = read( new_socket , usrname, 1024);
  valread = read( new_socket , pass, 1024);

  strcpy(id.usrname, usrname);
  strcpy(id.password, pass);
  // printf("%s\n", id.usrname);
  // printf("%s\n", id.password);

  FILE *f;
  f = fopen("akun.txt", "a+");

  if(strcmp(loginRegister, "login") == 0)
  {
    struct identitas checkID;
    bool loginSuccess = false;
    char loginCheck[100] = "";
    while(fread(&checkID,sizeof(struct identitas), 1, f))
    {
      if(strcmp(id.usrname, checkID.usrname) == 0 && strcmp(id.password, checkID.password) == 0)
      {
        printf("Auth Success\n");
        loginSuccess = true;
        strcat(loginCheck, "Login Success");
        break;
      }
    }
    if(!loginSuccess)
    {
      printf("Auth Failed\n");
      strcat(loginCheck, "Login Failed");
      send(new_socket , loginCheck , strlen(loginCheck) , 0 );
      screen1Handler(&new_socket);
      pthread_cancel(pthread_self());
    }
    else
      send(new_socket , loginCheck , strlen(loginCheck) , 0 );
    //printf("loginboi\n");
  }
  else if(strcmp(loginRegister, "register") == 0)
  {
    pthread_mutex_lock(&lock);

    //printf("regisboi\n");

    printf("List account :\n");
    struct identitas checkID;
    while(fread(&checkID,sizeof(struct identitas), 1, f))
    {
      printf("Username: %s\nPassword: %s\n\n", checkID.usrname, checkID.password);
    }
    printf("Username: %s\nPassword: %s\n\n", id.usrname, id.password);
    char regis[100] = "Register Success";
    send(new_socket , regis , strlen(regis) , 0 );
    fwrite(&id, sizeof(struct identitas), 1, f);

    pthread_mutex_unlock(&lock);
  }
  else
  {
    strcat(msgServer, "Salah memasukkan command");
  }
  fclose(f);
  screen2Handler(new_socket);
}



void playHandler()
{

}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    pthread_t thread[MAXROOM * 2];
    while (true)
    {
      addrlen = sizeof(address);
      if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
          perror("accept");
          exit(EXIT_FAILURE);
      }
      int *p_client_socket = (int *) malloc(2 * sizeof(int));
      socketIDs[players] = new_socket;


      pthread_create(&thread[players], NULL, screen1Handler, &new_socket);
      players++;
    }
    return 0;
}
