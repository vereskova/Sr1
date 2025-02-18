#define _POSIX_C_SOURCE 200809L

#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s fichier1 fichier2 \">\" fichier_dest\n", argv[0]);
        exit(0);
    }


    int redir = 0;
    int fdSort = STDOUT_FILENO;  
    int IndRedir = -1;        


    char *input_files[argc];
    int NbEntrFish = 0;

    for (int i = 1; i < argc; i++) 
    {
        if (strcmp(argv[i], ">") == 0) 
        {
            IndRedir = i;
            break;
        }
    }


    if (IndRedir != -1) {
        
        if (IndRedir + 1 >= argc) 
        {
            fprintf(stderr, "Erreur: pas de fichier après \">\"\n");
            exit(1);
        }

        char *fichier_dest = argv[IndRedir + 1];
        fdSort = open(fichier_dest, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        
        if (fdSort < 0) 
        {
            perror("open sortie");
            exit(2);
        }
        redir = 1;


    }


    for (int i = 1; i < argc; i++) 
    {
        if (IndRedir != -1 && i >= IndRedir) 
        {
            break;
        }
        input_files[NbEntrFish++] = argv[i];
    }

    for (int i = 0; i < NbEntrFish; i++) 
    {
        pid_t pid = fork();
        if (pid < 0) 
        {
            perror("fork");
            exit(3);
        }

        else if (pid == 0) 
        {
            
            
            // Fils

            if (redir) 
            {
                // Redirige la sortie standard vers fdSort
                if (dup2(fdSort, STDOUT_FILENO) < 0) 
                {
                    perror("dup2");
                    exit(4);
                }
            }

            // Ouvrir le fichier en lecture
            int fd_in = open(input_files[i], O_RDONLY);
            
            if (fd_in < 0) 
            {
                perror(input_files[i]);
                exit(5);
            }

            char buffer[4096];
            ssize_t NbLus;
            while ((NbLus = read(fd_in, buffer, sizeof(buffer))) > 0) 
            {
                if (write(STDOUT_FILENO, buffer, NbLus) != NbLus) 
                {
                    perror("write");
                    close(fd_in);
                    exit(6);
                }
            }
            
            if (NbLus < 0) 
            {
                perror("read");
            }

            close(fd_in);

            _exit(EXIT_SUCCESS);
        }

        else 
        {

            int stats;
            waitpid(pid, &stats, 0);

        }
    }

    if (redir) 
    {
        close(fdSort);
    }

   exit (0);
}
