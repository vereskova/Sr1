#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>



int main (void)
{
pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        execlp("ls", "ls", "-la", NULL);
        printf("ls");
        exit(2);
    }

    wait(NULL);
    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    else if (pid == 0)
    {
        execlp("date", "date", NULL);
        printf("date");
        exit(2);
    }
    wait(NULL);
    exit(0); 
}