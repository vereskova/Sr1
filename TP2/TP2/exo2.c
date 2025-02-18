#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    
#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>   


int main(int argc, char *argv[])
{

    if (argc < 3) {
        fprintf(stderr, "Usage: %s n [valeur] fichier\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Erreur n doit être un entier positif\n");
        exit(1);
    }


    int modeLecture = 1; 
    int valeur = 0;
    char *nom_fichier = NULL;


    if (argc == 3) 
    {
        nom_fichier = argv[2];
    }
    else if (argc == 4) 
    {
        modeLecture = 0;
        valeur = atoi(argv[2]);
        nom_fichier = argv[3];
    }
    else 
    {
        fprintf(stderr, "Trop d'arguments.\n");
        exit(2);
    }


    int fd;


    if (modeLecture) 
    {
        fd = open(nom_fichier, O_RDONLY);
    } 
    else 
    {
        fd = open(nom_fichier, O_RDWR);
    }

    if (fd < 0)
    {
        perror("open");
        exit(3);
    }




    off_t offset = (off_t)(n - 1) * sizeof(int);
    off_t ret = lseek(fd, offset, SEEK_SET);


    if (ret == (off_t)-1) 
    {
        perror("lseek");
        close(fd);
        exit(4);
    }

    if (modeLecture) 
    {
        int valLu;
        ssize_t nbLus = read(fd, &valLu, sizeof(int));
       
        if (nbLus < 0) 
        {
            perror("read");
            close(fd);
            exit(5);
        }
        
        else if (nbLus < (ssize_t)sizeof(int)) 
        {
            fprintf(stderr, "Le fichier ne contient pas l'entier numéro %d.\n", n);
            close(fd);
            exit(6);
        }

        printf("L'entier n°%d dans le fichier %d\n", n, valLu);
    }
   
    else 
    {
        ssize_t nbEcrits = write(fd, &valeur, sizeof(int));
        if (nbEcrits < 0) {
            perror("write");
            close(fd);
            exit(7);
        }
        else if (nbEcrits < (ssize_t)sizeof(int)) {
            fprintf(stderr, "Impossible d'écrire 4 octets\n");
            close(fd);
            exit(8);
        }
        printf("J'ai écrit la valeur %d à la position %d\n", valeur, n);
    }

    close(fd);
    exit (0);
}
