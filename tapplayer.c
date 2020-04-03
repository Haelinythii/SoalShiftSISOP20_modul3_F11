#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <termios.h>
#include <pthread.h>
#define PORT 8080

int health = 100;
pthread_t tempThread;

static struct termios oldt, newt;

void screen1(int sock);

void* mulaiMatchTap(void* args)
{
  tempThread = pthread_self();

  int sock = *(int *) args;
  int c;

  tcgetattr( STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  while((c=getchar())!= '-')
  {
    if(c == ' ')
    {
      char hp[10] = "hit";
      //sprintf(hit, "%d", health)
      send(sock , hp , strlen(hp) , 0 );
      printf("hit !!\n");
    }
    putchar(c);
    if(health <= 0)
      break;
  }

  printf("\n");
}

void* mulaiMatchRead(void* args)
{
  int sock = *(int *) args, valread;
  char hitRead[100];
  while (health > 0)
  {
    valread = read( sock , hitRead, 1024);
    if(strcmp(hitRead, "Game berakhir kamu kalah") == 0)
    {
      printf("%s\n", hitRead);
      pthread_cancel(tempThread);
      send(sock , hitRead , strlen(hitRead) , 0 ); // send biar ga stuck
      break;
    }
    else if (strcmp(hitRead, "Game berakhir kamu menang") == 0)
    {
      printf("%s\n", hitRead);
      pthread_cancel(tempThread);
      break;
    }
    health -= 10;
    printf("%d\n", health);
  }
}

void screen2(int sock)
{
  health = 100; //reset health

  int valread;
  char choiceScreen2[100];
  printf("1. Find Match\n" );
  printf("2. Logout\n" );
  printf("   Choices : " );
  scanf("%s", choiceScreen2);
  send(sock , choiceScreen2 , strlen(choiceScreen2) , 0 );
  if(strcmp(choiceScreen2, "logout") == 0)
  {
    screen1(sock); //balik ke screen1
  }
  else if(strcmp(choiceScreen2, "find") == 0)
  {
    char a[100] = "";
    do {

      valread = read( sock , a, 1024);
      printf("Waiting for player ...\n");
      /* code */
    } while(strcmp(a, "Waiting for player ...") == 0);

    printf("Game dimulai silahkan tap tap secepat mungkin !!\n");
    //send signal
    //loop waiting for signal back
    pthread_t thread[2];

    pthread_create(&thread[0], NULL, mulaiMatchTap, &sock);
    pthread_create(&thread[1], NULL, mulaiMatchRead, &sock);
    pthread_join(thread[1], NULL);
    printf("lanjut\n");

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);//ngembalikan settingan stdin

    screen2(sock);
    //mulaiMatch(sock);
  }
  else
  {
    screen2(sock);
  }
}


void screen1(int sock)
{
  int valread;
  char choice[100], usrname[100] = "", pass[100] = "";
  printf("1. Login\n");
  printf("2. Register\n");
  printf("   Choices : ");

  scanf("%s", choice);
  send(sock , choice , strlen(choice) , 0 );

  if(!strcmp(choice, "login") == 0 && !strcmp(choice, "register") == 0)
  {
    screen1(sock);
    return;
  }

  printf("Username : ");
  getchar();
  scanf("%[^\n]s", usrname);
  //printf("%s\n", usrname);
  send(sock , usrname , strlen(usrname) , 0 );
  printf("Password : ");
  getchar();
  scanf("%[^\n]s", pass);
  send(sock , pass , strlen(pass) , 0 );

  char msgAuth[100] = "";
  valread = read( sock , msgAuth, 1024);
  printf("%s\n", msgAuth);
  if(strcmp(msgAuth, "Login Success") == 0 || strcmp(msgAuth, "Register Success") == 0)
  {
    screen2(sock);
  }
  else
  {
    screen1(sock);
  }
}



int main(int argc, char const *argv[]) {

    // static struct termios oldt;
    // tcgetattr( STDIN_FILENO, &oldt);
    // oldt.c_lflag &= ICANON;
    // tcsetattr( STDIN_FILENO, TCSANOW, &oldt);


    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    //while (1)
    //{
      //char loginScreen[100] = {0};
      //valread = read( sock , loginScreen, 1024);
      //printf("%s", loginScreen);
      screen1(sock);

      // char msgUsername[100]
      // valread = read( sock , msgUsername, 1024);
      // char msgPassword[100]
      // valread = read( sock , msgPassword, 1024);
      //
      // char buffer[1024] = {0};
      //
      // //printf("Hello message sent\n");
      // valread = read( sock , buffer, 1024);
      // printf("%s\n",buffer );
    //}
}
