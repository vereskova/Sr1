// Exercice 1.2 :

// écrire un programme C ANSI qui prend en paramètre le nom d’une
// variable d’environnement et qui affiche à l’écran sa valeur.

#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <variable>\n", argv[0]);
        return 1;
    }

    char *value = getenv(argv[1]);
    if (value == NULL)
    {
        fprintf(stderr, "La variable d'environnement %s n'existe pas.\n", argv[1]);
        return 1;
    }

    printf("%s=%s\n", argv[1], value);
    return 0;
}