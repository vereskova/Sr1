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

int main(void)
{
    struct stat infos;
    const char *chemin = "fichier.txt"; 

    if (lstat(chemin, &infos) == -1) {
        perror("lstat");
        exit(EXIT_FAILURE);
    }


    printf("%s ", chemin);
    Affiche_inode(&infos);

    return 0;
}