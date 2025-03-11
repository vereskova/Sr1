#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define N 4 // Количество дочерних процессов

int est_premier (unsigned long long int n) {
    if ((n>1) && (n==2 || n==3 || ((n%2)&&(n%3)))) {
        unsigned long long int i,y;
        y=sqrt(n)+1;
        i=5;
        while(i<y) {
            if (n%i == 0)
                return 0;
            i+=2;
            if (n%i == 0)
                return 0;
            i+=4;
        }
        return 1;
    }
    return 0;
}

void child_process(int pipe_fd, int id) {
    close(pipe_fd + 1); // Закрываем запись
    unsigned long long int num;
    int count = 0;
    while (read(pipe_fd, &num, sizeof(num)) > 0) {
        if (est_premier(num)) {
            printf("[Fils %d] %llu est premier\n", id, num);
            count++;
        } else {
            printf("[Fils %d] %llu\n", id, num);
        }
    }
    close(pipe_fd);
    exit(count);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s n1 n2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pids[N];
    for (int i = 0; i < N; i++) {
        if ((pids[i] = fork()) == 0) {
            close(pipe_fd[1]); // Закрываем запись
            child_process(pipe_fd[0], i);
        }
    }
    
    close(pipe_fd[0]); // Закрываем чтение
    for (int i = 1; i < argc; i++) {
        unsigned long long int num = strtoull(argv[i], NULL, 10);
        write(pipe_fd[1], &num, sizeof(num));
    }
    close(pipe_fd[1]); // Закрываем запись

    int total_primes = 0;
    for (int i = 0; i < N; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status)) {
            total_primes += WEXITSTATUS(status);
        }
    }
    printf("Il y a %d nombres premiers.\n", total_primes);

    return EXIT_SUCCESS;
}
