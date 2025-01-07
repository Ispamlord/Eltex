#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MAX_LEN 256

volatile sig_atomic_t flag = 1;

void sigint_handler(int sig) {
    flag = 0;
}

int main() {
    int shmid;
    int semid;
    int* shmp;

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    if ((shmid = shmget(IPC_PRIVATE, MAX_LEN * sizeof(int), IPC_CREAT | 0666)) == -1) {
        perror("shmid");
        exit(EXIT_FAILURE);
    }
    if ((shmp = shmat(shmid, NULL, 0)) == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    if ((semid = semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT)) == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        struct sembuf lock_parent = { 0, -1, 0 };
        struct sembuf unlock_parent = { 1, 1, 0 };

        while (flag) {
            if (semop(semid, &lock_parent, 1) == -1) {
                if (errno == EINTR)
                    break;
                perror("semop");
                exit(EXIT_FAILURE);
            }

            int min = shmp[0];
            int max = shmp[0];
            int count = shmp[MAX_LEN];
            for (size_t i = 0; i < count; i++) {
                if (shmp[i] > max) {
                    max = shmp[i];
                }
                if (shmp[i] < min) {
                    min = shmp[i];
                }
            }
            shmp[0] = min;
            shmp[1] = max;

            if (semop(semid, &unlock_parent, 1) == -1) {
                if (errno == EINTR)
                    break;
                perror("semop");
                exit(EXIT_FAILURE);
            }
        }

        if (shmdt(shmp) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
    }
    else {
        struct sembuf lock_child = { 0, 0, 0 };
        struct sembuf unlock_child = { 0, 1, 0 };
        struct sembuf unlock = { 1, -1, 0 };

        while (flag) {
            sleep(1);
            if (semop(semid, &lock_child, 1) == -1) {
                if (errno == EINTR)
                    break;
                perror("semop");
                exit(EXIT_FAILURE);
            }

            srand(time(NULL));
            int count = rand() % MAX_LEN;
            int i = 0;
            while (i < count) {
                shmp[i] = rand() % 1000;
                i++;
            }
            shmp[MAX_LEN] = count;

            if (semop(semid, &unlock_child, 1) == -1) {
                if (errno == EINTR)
                    break;
                perror("semop");
                exit(EXIT_FAILURE);
            }
            if (semop(semid, &unlock, 1) == -1) {
                if (errno == EINTR)
                    break;
                perror("semop");
                exit(EXIT_FAILURE);
            }

            printf("Минимальное число: %d\nМаксимальное число: %d\n", shmp[0], shmp[1]);
        }
        if (shmdt(shmp) == -1) {
            perror("shmdt");
            exit(EXIT_FAILURE);
        }
        if (semctl(semid, 0, IPC_RMID, 0) == -1) {
            perror("semctl");
            exit(EXIT_FAILURE);
        }
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}