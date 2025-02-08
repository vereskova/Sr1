// Exercice 2.1 :
// Écrire un programme qui crée un processus fils.
// Après la création, le processus père doit afficher son propre identificateur.
// Le processus fils doit afficher l’identificateur de son père ainsi que son
// identificateur.

#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror ("fork");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0)
    {
        printf("L identificateur du processus pere = %d", getpid());
    }
    
    else
    {
        printf("L identificateur du processus pere dans processus fils = %d", getppid());
        printf("L identificateur du processus fils = %d", getpid());
    }

    return 0;
}

