#define _POSIX_C_SOURCE 200809L
#include <stdio.h>    
#include <stdlib.h>   
#include <unistd.h>   
#include <sys/types.h>
#include <sys/wait.h> 
#include <fcntl.h>    
 
int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s N MAX_BASSIN\n", argv[0]);
        exit(EXIT_FAILURE);
    }
 
    int N = atoi(argv[1]);
    int MAX_BASSIN = atoi(argv[2]);
    if (N <= 0 || MAX_BASSIN <= 0) {
        fprintf(stderr, "N и MAX_BASSIN должны быть > 0\n");
        exit(EXIT_FAILURE);
    }
 
    int Remplir[2], Etat[2];
    if (pipe(Remplir) == -1) {
        perror("pipe Remplir");
        exit(EXIT_FAILURE);
    }
    if (pipe(Etat) == -1) {
        perror("pipe Etat");
        exit(EXIT_FAILURE);
    }
 
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
 
     if (pid == 0) {
        //Дочерний процесс
        close(Remplir[1]); // закрываем запись
        close(Etat[0]);    // закрываем чтение (в Etat)
 
        int val;
        int bassin = 0;
 
        while (read(Remplir[0], &val, sizeof(int)) > 0) {
            if (val > 0) {
                bassin += val;
                for (int i = 0; i < val; i++) {
                    printf("*");
                }
                fflush(stdout);
 
                // Если бассейн превышен
                if (bassin >= MAX_BASSIN) {
                    // Сообщаем отцу (пишем в Etat)
                    if (write(Etat[1], &bassin, sizeof(int)) == -1) {
                        perror("write Etat");
                    }
                }
            } 
            else if (val < 0) {
                // Сброс
                bassin = 0;
                printf("\n"); 
                fflush(stdout);
            }
        }
 
        close(Remplir[0]);
        close(Etat[1]);
        exit(1);
 
    } else {
        //Родительский процесс
        close(Remplir[0]); // не нужно читать из Remplir
        close(Etat[1]);    // не нужно писать в Etat
 
        // Переводим Etat[0] (чтение) в неблокирующий режим
        int f_flags = fcntl(Etat[0], F_GETFL);
        f_flags |= O_NONBLOCK; 
        fcntl(Etat[0], F_SETFL, f_flags);
 
        // Посылаем "1" N раз в Remplir
        for (int i = 0; i < N; i++) {
            int val = 1;
            if (write(Remplir[1], &val, sizeof(int)) == -1) {
                perror("write Remplir");
                close(Remplir[1]);
                close(Etat[0]);
                exit(EXIT_FAILURE);
            }
 
            // Пытаемся прочитать из Etat (неблокирующий)
            int bassinValue;
            ssize_t bytesRead = read(Etat[0], &bassinValue, sizeof(int));
            if (bytesRead > 0) {
                // Если мы что-то прочитали, значит есть сигнал переполнения
                // => отправляем -1 (сброс)
                int vidange = -1;
                if (write(Remplir[1], &vidange, sizeof(int)) == -1) {
                    perror("write Remplir");
                }
            }
 
            sleep(1);
        }
 
        // Закрываем запись в Remplir
        close(Remplir[1]);
        // Закрываем чтение Etat
        close(Etat[0]);
 
        // Ждем, когда потомок завершится
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
 