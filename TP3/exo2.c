// Сначала нужно расписать свич, какие типы файлов и репозиториев есть. 

#define _POSIX_C_SOURCE 200809L

#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>
#include <time.h>



 const char* TypeFichier(const struct stat *Infos)
{
    switch (Infos->st_mode & S_IFMT) {
        case S_IFREG:  return "fichier ordinaire";
        case S_IFDIR:  return "répertoire";
        case S_IFLNK:  return "lien symbolique";
        case S_IFBLK:  return "périphérique bloc";
        case S_IFCHR:  return "périphérique caractère";
        case S_IFIFO:  return "fifo (tube nommé)";
        case S_IFSOCK: return "socket";
        default:       return "type inconnu";
    }
}


void Affiche_inode (struct  stat * Infos){
    
    char *date = ctime(&Infos->st_mtime);

    if (date != NULL) {
        size_t len = strlen(date);
        if (len > 0 && date[len - 1] == '\n') 
        {
            date[len - 1] = '\0';
        }
    }
     
    else 
    {
        date = "Date inconnue";
    }


    printf("%-20s %10ld octets %s\n", TypeFichier(Infos), (long) Infos->st_size, date);
}

int main(int argc, char * argv[])
{
    struct stat infos;

    if (argc < 2)
    {
        printf("Usage: %s fichier\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    { 

        for (int i = 1;  i < argc; i++)
        {
            // Пытаемся получить информацию о файле
            if (lstat(argv[i], &infos) == -1) {
            perror("lstat");
            // Вместо завершения программы можно пропустить файл и перейти к следующему
            continue;
        }
            printf ("%s ", argv[i]);
            Affiche_inode(&infos);
        }

    }

    return 0;
}