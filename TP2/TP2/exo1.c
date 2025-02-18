#define _POSIX_C_SOURCE 200809L

#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    
#include <stdio.h>   
#include <stdlib.h> 

void ecrire_dans_stdout(char nom_fich[])
{
    int fd = open(nom_fich, O_RDONLY);
    
    if (fd == -1)
    {
        perror("open");
        return;
    }

    char buffer[4096];

    ssize_t NbLus;

    while ((NbLus = read(fd, buffer, sizeof(buffer))) > 0) 
    {
        ssize_t NbEcrits = write(STDOUT_FILENO, buffer, NbLus);

        if (NbEcrits < 0) 
        {
            perror("NbEcrits");
            break;
        }
    }

    if (NbLus < 0) 
    {
        perror("NbLus");
    }

    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s nom_fichier\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ecrire_dans_stdout(argv[1]);
    return 0;
}