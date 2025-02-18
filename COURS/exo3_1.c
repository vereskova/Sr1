// ; Exercice 3.1

// ; écrire un programme C qui effectue la copie du contenu d’un fichier par
// ; blocs de 512 octets.
// ; Ce programme prend deux arguments : la désignation du fichier source et
// ; la désignation du fichier destination.
// ; Si le fichier destination n’existe pas, il doit être créé avec le droit en
// ; lecture pour le propriétaire et le groupe, et le droit en écriture pour le
// ; propriétaire. S’il existe, son contenu doit être « écrasé ».
// ; Le programme doit gérer les codes de retour en cas d’erreur (mauvais
// ; nombre de paramètres, fichier inaccessible, erreurs de lecture-écriture...).

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define TAILLE_BLOC 512

int main(int argc, char * argv[])
{
    if (argc != 3 )
    {
        perror ("mauvais nombre de paramètres");
        exit (1);
    }

    int source = open(argv[1], O_RDONLY);

    if (source == -1)
    {
        perror ("erreurs de lecture argv[1]");
        exit (2);
    }

    int destination = open (argv[2], O_CREAT|O_WRONLY|O_TRUNC|S_IRUSR|S_IRGRP|S_IWUSR);

    if (destination == -1)
    {
        perror ("erreurs de lecture argv[2]");
        close (source);
        exit (3);
    }

    char bloc[TAILLE_BLOC];
    ssize_t nbLus, nbEcrits;
    nbLus = 0;
    nbEcrits = 0;

    while ((nbEcrits == nbLus) && (nbLus = read(source, bloc, TAILLE_BLOC))>0)
    {
        nbEcrits = write(destination, bloc, nbLus);
        int exit_status = 0;
        if (nbLus < 0)
        {
            perror(argv[1]);
            exit_status = 4;
        }

        if ((nbEcrits < 0 )&&(nbEcrits < nbLus))
        {
            if (nbEcrits < 0 )
            {
                perror(argv[2]);
            }
            else
            {
                printf (stderr, "Ecriture interrompue dans %s \n", argv[2]);
            }
            exit_status = 5;
            close (source);
            if (close(destination) == 1)
            {
                perror (argv[2]);
                exit_status = 6;
            }
            exit (exit_status);
        }
    }
 



}
