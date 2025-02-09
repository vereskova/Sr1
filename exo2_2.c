// Exercice 2.2 :

// Écrire un programme qui permet à quatre processus frères – numérotés 1, 2, 3 et 4 – de
// constituer avec le père – numéroté 0 – un anneau unidirectionnel. Entre ces processus,
// on définit la relation de successeur de la manière suivante :
// successeur(i)=i+1 pour i=0,1,2,3.
// successeur(4)=0
// Mais on donnera comme valeur de successeur à chaque processus, le pid de son
// successeur (et pas son numéro dans l’anneau, trop facile!).
// Chaque processus a donc un numéro dans l’anneau et un successeur qui correspond
// au pid du suivant dans l’anneau. Il affiche N fois (N : constante symbolique définie
// dans le programme) son numéro, son pid et son successeur, puis retourne un compte
// rendu égal à son numéro (pas son pid).
// Le père attend, avant de se terminer, la fin de tous ses fils et, à chaque terminaison
// d’un fils, il affiche le pid et le code de retour de ce fils.

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 1  // сколько раз каждый процесс выводит свои данные

int main(void)
{
    // Отец (номер 0) запускается первым:
    int   ring_number = 0;            // 0 - это отец
    pid_t father_pid = getpid();      // PID отца
    pid_t child_pid  = 0;

    /*
      Логика "цепочки":
      - Процесс с ring_number=0 (отец) создаёт 1-го сына (ring_number=1).
      - Тот, в свою очередь, создаёт 2-го (ring_number=2).
      - Тот — 3-го (ring_number=3).
      - Тот — 4-го (ring_number=4).
      - Каждый знает PID только что порождённого потомка, следовательно может 
        выставить для себя "successor_pid = pid_ребёнка".
      - Последний (ring_number=4) в качестве "successor_pid" укажет PID отца 
        (father_pid), замыкая кольцо.
    */

    // 1) Первый fork — пусть делает отец (ring_number=0)
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (child_pid == 0) {
        // Мы в потомке => это процесс с ring_number=1
        ring_number = 1;
        // 2) Создаём следующего
        child_pid = fork();
        if (child_pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (child_pid == 0) {
            // Теперь ring_number=2
            ring_number = 2;
            // 3) Следующий
            child_pid = fork();
            if (child_pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (child_pid == 0) {
                // Теперь ring_number=3
                ring_number = 3;
                // 4) Последний потомок
                child_pid = fork();
                if (child_pid < 0) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                if (child_pid == 0) {
                    // ring_number=4
                    ring_number = 4;
                    // Последний не порождает детей
                }
            }
        }
    }

    // Теперь у нас 5 процессов с ring_number = 0..4:
    pid_t my_pid       = getpid();
    pid_t successor_pid = 0;

    if (ring_number < 4) {
        // Пока не дошли до "4", преемник = child_pid (тот, что только что породили)
        // Если у нас ring_number=0 (отец), child_pid — это PID процесса #1
        // Если ring_number=1, то child_pid — PID процесса #2 и т.д.
        successor_pid = child_pid;
    } else {
        // ring_number=4 => замыкаем кольцо на отца
        successor_pid = father_pid;
    }

    // Каждый процесс выводит свою информацию N раз
    for (int i = 0; i < N; i++) {
        printf(
            "Ring=%d | PID=%d | SuccessorPID=%d\n",
            ring_number, (int)my_pid, (int)successor_pid
        );
        fflush(stdout);
        sleep(1);  
    }

    // Код возврата (exit) = ring_number.
    // Но отец будет делать wait() на всех потомков, поэтому:
    if (ring_number == 0) {
        // Отец ожидает завершения 4 детей (общее число потомков в цепочке)
        int status;
        pid_t ended;
        while ((ended = wait(&status)) > 0) {
            if (WIFEXITED(status)) {
                printf(
                    "Process with PID=%d finished with code=%d\n",
                    (int)ended,
                    WEXITSTATUS(status)
                );
            }
        }
        printf("Father finishes.\n");
        return 0;
    } else {
        // Дочерние процессы завершаются со своим номером
        exit(ring_number);
    }
}
