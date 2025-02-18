#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void affich_infos_processeur()
{
    printf ("%d\n", getpid());
    printf ("%d\n", getppid());
    printf ("%d\n", getuid());
    printf ("%d\n", getgid());
}

int main(void)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        printf("fils\n");
        affich_infos_processeur();
        exit(2);
    }
    else if (pid > 0 ) 
    {
        wait(NULL);
        printf("\n\npere\n");
        affich_infos_processeur();
        exit(0); 
    }
}

