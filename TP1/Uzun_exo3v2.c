#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "decoupe.h" 

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %scommande1 commande2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t *tousMesFils = malloc((argc - 1) * sizeof(pid_t));
    if (tousMesFils == NULL)
    {
        perror("malloc");
        exit(1);
    }


    for (int i = 1; i < argc; i++)     // Création les fils
    {
        char *pMots[NBMOTSMAX + 1];
        char copie[1024];
        snprintf(copie, sizeof(copie), "%s", argv[i]);
        
        Decoupe(copie, pMots);

        pid_t pidFils = fork();
        if (pidFils < 0)
        {
            perror("fork");
            exit(2);
        }
        else if (pidFils == 0)
        {

            printf("[%d] Je lance %s :\n", (int)getpid(), argv[i]);

            execvp(pMots[0], pMots);
            perror("execvp");
            exit(3);
        }
        else
        {

            tousMesFils[i - 1] = pidFils;             // Code du père
            printf("[%d] J'ai délégué %s à %d.\n", (int)getpid(), argv[i], (int)pidFils);
        }
    }

    for (int i = 1; i < argc; i++)
    {
        int stats;
        pid_t pidTermine = waitpid(tousMesFils[i - 1], &stats, 0);
        if (pidTermine > 0)
        {
            printf("[%d] %d terminé.\n", (int)getpid(), (int)pidTermine);
        }
        else
        {
            perror("waitpid");
        }
    }

    printf("[%d] J'ai fini.\n", (int)getpid());
    return EXIT_SUCCESS;
}
