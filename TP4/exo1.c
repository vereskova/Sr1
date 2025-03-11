#define _POSIX_C_SOURCE 200809L
#include <stdio.h>  
#include <stdlib.h> 
#include <unistd.h>   
#include <sys/types.h>
#include <sys/wait.h> 
 
int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "N должен быть положительным целым числом\n");
        exit(EXIT_FAILURE);
    }
 
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
 
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
 
    if (pid == 0) {
        // Дочерний процесс
        close(tube[1]); // Закрываем сторону записи
 
        int val;
        // Читаем, пока данные поступают
        while (read(tube[0], &val, sizeof(int)) > 0) {
            printf("[Fils] %d\n", val);
        }
 
        close(tube[0]); 
        // Завершаемся с кодом возврата 1
        exit(1);
 
    } else {
        // Родительский процесс
        close(tube[0]); // Закрываем сторону чтения
 
        // Отправка чисел 1..N по одному в секунду
        for (int i = 1; i <= N; i++) {
            if (write(tube[1], &i, sizeof(int)) == -1) {
                perror("write");
                close(tube[1]);
                exit(EXIT_FAILURE);
            }
            sleep(1);
        }
 
        close(tube[1]); // Закрываем сторону записи
 
        // Ждем завершения дочернего процесса
        int status;
        waitpid(pid, &status, 0);
 
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            printf("[Père] : Mon fils %d est terminé avec le code %d.\n", pid, code);
        } else {
            printf("[Père] : Mon fils %d завершился ненормально.\n", pid);
        }
 
        exit(EXIT_SUCCESS);
    }
 }
 