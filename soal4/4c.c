#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int fd1[2];
int fd2[2];

void execLS ()
{
	dup2(fd1[1], 1);
	close(fd1[0]);
	close(fd1[1]);
	char *argv[] = {"ls", NULL};
	execv("/bin/ls", argv);
}

void execWC()
{
	dup2(fd1[0], 0);
	close(fd1[0]);
	close(fd1[1]);
	char *argv[] = {"wc", "-l", NULL};
	execv("/usr/bin/wc", argv);
}

int main()
{
	if (pipe(fd1)==-1)
	{
		fprintf(stderr, "Pipe Failed" );
		return 1;
	}
	if (pipe(fd2)==-1)
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
