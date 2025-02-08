// Exercice 1.3 

// écrire un programme C ANSI qui prend en paramètre une liste de nombres
// entiers et affiche à l’écran leur valeur après les avoir convertis en int.
// Le programme doit vérifier que le paramètre est bien un entier.
// Utiliser la fonction strtol pour convertir la chaîne de caractères de chaque
// argument du programme en int. En effet, cette fonction gère correctement
// les erreurs contrairement à la fonction atoi.

#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char *argv[])
{
 /* On commence à 1, car argv[0] est le nom du programme */
    for(int i = 1; i < argc; i++)
    {
        char *endptr = NULL;
        errno = 0;  /* Réinitialisation de errno avant l'appel à strtol */
        
        /* Conversion en base 10 */
        long val = strtol(argv[i], &endptr, 10);

        /* Vérification des différentes erreurs possibles :
           - errno != 0 indique une erreur système (overflow, etc.)
           - *endptr != '\0' indique que la chaîne n'a pas été entièrement convertie
           - vérification supplémentaire si le long dépasse la plage de int */
        if ((errno != 0) || (*endptr != '\0') || (val < INT_MIN) || (val > INT_MAX))
        {
            fprintf(stderr, "Erreur : \"%s\" n'est pas un entier valide.\n", argv[i]);
        }
        else
        {
            /* Conversion sûre en int */
            int nombre = (int)val;
            printf("Argument %d : %d\n", i, nombre);
        }
    }
    return 0;
}