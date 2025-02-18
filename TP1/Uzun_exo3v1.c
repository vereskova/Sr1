#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "decoupe.h" 

int main(int argc, char *argv[])
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s commande1 commande2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pidFils;
    int stats;

    for (int i = 1; i < argc; i++)
    {
        char *pMots[NBMOTSMAX + 1];
        char copie[1024];
        snprintf(copie, sizeof(copie), "%s", argv[i]);
        
        Decoupe(copie, pMots);

        pidFils = fork();

        if (pidFils < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pidFils == 0)
        {

            printf("[%d] Je lance %s :\n", (int)getpid(), argv[i]);             // Code du fils

            execvp(pMots[0], pMots);

            perror("execvp");
            exit(2);
        }
        else
        {

            printf("[%d] J'ai délégué %s à %d. J'attends sa fin...\n", (int)getpid(), argv[i], (int)pidFils);             //Code du père


            waitpid(pidFils, &stats, 0);             // Attendre la fin du fils

            printf("[%d] %d terminé.\n", (int)getpid(), (int)pidFils);
        }
    }

    printf("[%d] J'ai fini.\n", (int)getpid());
    return EXIT_SUCCESS;
}