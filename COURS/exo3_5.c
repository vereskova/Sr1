// Exercice 3.5

// écrire un programme C qui lance l’exécution d’une commande en réalisant les
// redirections du type : commande < fich_entree >> fich_sortie
// Il prend trois arguments : la commande, la désignation du fichier vers lequel doit être
// redirigée l’entrée standard et la désignation du fichier vers lequel doit être redirigée la
// sortie standard (avec ajout en fin de fichier).
// L’exécution de la commande doit être déléguée à un processus Fils.
// Les redirections doivent être réalisées dans le Père par duplication des descripteurs de
// fichiers. Si le fichier de sortie n’existe pas, il doit être créé avec le droit en lecture pour
// tous et le droit en écriture pour le propriétaire.
// Le programme doit gérer les codes de retour en cas d’erreur (mauvais nombre de
// paramètres, fichier inaccessible, etc.).

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char * argv[])
{
    if (argc != 4 )
    {
        perror ("mauvais nombre de paramètres");
        exit (1);
    }
    int entree, sortie;
    entree = open (argv[2], O_RDONLY);
    if (entree == -1 )
    {
        perror ("argv[2]");
        exit (2);
    }

    sortie = open (argv[3], O_CREAT|O_WRONLY|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if (sortie == -1 )
    {
        perror ("argv[3]");
        close(entree);
        exit (3);
    }
    if (dup2(entree, STDIN_FILENO) == -1)
    {
        perror("dup2 entree");
        close(entree);
        close(sortie);
        exit(4);
    }

    if (dup2(sortie, STDOUT_FILENO) == -1)
    {
        perror("dup2 sortie");
        close(entree);
        close(sortie);
        exit(5);
    }

    pid_t pid_fils = fork();

    if (pid_fils == -1)
    {
        perror(fork);
        close(entree);
        close(sortie);
        exit(6);
    }

    if (pid_fils == 0)
    {
        execl(argv[1], argv[1], NULL);
        perror(argv[1]);
        close(entree);
        close(sortie);
        exit(7);
    }

    wait(NULL);
    close(entree);
    close(sortie);
    exit(0);
}