#define _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Рекурсивная функция поиска пустых файлов
void find_empty_files(const char *path, int *count) {
    struct stat infos;
    
    // Получаем информацию о path
    if (lstat(path, &infos) == -1) {
        // Если ошибка при lstat, выведем предупреждение и вернемся
        perror("lstat");
        return;
    }

    // Если path — директория, заходим внутрь
    if (S_ISDIR(infos.st_mode)) {
        DIR *rep = opendir(path);
        if (!rep) {
            perror("opendir");
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(rep)) != NULL) {
            // Пропускаем . и ..
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            // Формируем полный путь к элементу
            char subpath[4096];
            snprintf(subpath, sizeof(subpath), "%s/%s", path, entry->d_name);
            
            // Рекурсивно обходим поддиректории/файлы
            find_empty_files(subpath, count);
        }

        closedir(rep);
    }
    // Если path — обычный файл, проверяем, пустой ли он (size=0)
    else if (S_ISREG(infos.st_mode)) {
        if (infos.st_size == 0) {
            printf("%s\n", path); // Выводим путь к файлу
            (*count)++;           // Увеличиваем счетчик
        }
    }
    // Иначе просто ничего не делаем (необычные файлы, ссылки и т.п.)
}

int main(int argc, char *argv[]) {
    // Проверяем, что пользователь передал хотя бы 1 аргумент (директорию)
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <répertoire_racine>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int count = 0; // счетчик пустых файлов

    // Запускаем рекурсивный обход от argv[1]
    find_empty_files(argv[1], &count);

    // В конце выводим статистику
    printf("%d fichier(s) vide(s) trouvé(s) !\n", count);

    return 0;
}
