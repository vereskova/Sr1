// Exercice 1.1 :
// écrire un programme C ANSI qui affiche à l’écran ses arguments (un
// argument par ligne)


#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    for (int i = 1; i < argc; i++)
    {
        printf("Argument %d : %s\n", i, argv[i]);
    }
}