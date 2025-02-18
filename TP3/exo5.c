#define _POSIX_C_SOURCE 200809L

#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>
#include <time.h>
#include <pwd.h>    // <-- Добавляем для getpwuid()

// Функция определения типа файла:
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

// Функция вывода: тип, taille, propriétaire, date modif
void Affiche_inode(const struct stat *Infos)
{
    // Получаем дату в виде строки
    char *date = ctime(&Infos->st_mtime);
    if (date != NULL) {
        size_t len = strlen(date);
        if (len > 0 && date[len - 1] == '\n') {
            date[len - 1] = '\0';  // Убираем перевод строки в конце
        }
    } else {
        date = "Date inconnue";
    }

    // Получаем имя владельца
    struct passwd *pw = getpwuid(Infos->st_uid);
    const char *ownerName = (pw != NULL) ? pw->pw_name : "inconnu";

    // Выводим на одной строке: 
    //   тип, taille (en octets), ownerName, date
    // Пример формата: 
    //   fichier ordinaire   4131 octets  wozniak  Fri Jan 26 23:19:55 2024
    printf("%-20s %10ld octets %s %s\n",
           TypeFichier(Infos),
           (long) Infos->st_size,
           ownerName,
           date);
}

int main(int argc, char * argv[])
{
    struct stat infos;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <fichier1> [fichier2 ...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Обходим все переданные параметры
    for (int i = 1; i < argc; i++)
    {
        // Пытаемся получить информацию о файле
        if (lstat(argv[i], &infos) == -1)
        {
            perror("lstat");
            continue;
        }
        // Сначала выводим путь к файлу
        printf("%s ", argv[i]);
        // Затем вызываем Affiche_inode для печати деталей
        Affiche_inode(&infos);
    }

    return 0;
}
