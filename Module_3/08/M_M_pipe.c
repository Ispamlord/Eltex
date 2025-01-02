#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>
#include <fcntl.h>
#include<stdbool.h>
#include<signal.h>
#include <semaphore.h>

// Семафор для синхронизации
sem_t sem;

void Parent_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sleep(3);

        // Разрешаем дочернему процессу писать в канал
        sem_post(&sem);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Read from pipe: %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        // Блокируем доступ дочернего процесса
        sem_wait(&sem);
    }

    close(pipefd[0]);
    wait(NULL);  // Ожидаем завершения дочернего процесса
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);
    for (int i = 0; i < count; i++) {
        // Ждем, пока родительский процесс разрешит доступ
        sem_wait(&sem);

        int r = rand() % 100;
        printf("Write to pipe: %d\n", r);

        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            exit(1);
        }
        sleep(1);
    }
    close(pipefd[1]);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Enter 2 elements");
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);

    pid_t pid;
    int pipefd[2];

    // Инициализация семафора (0 - начальное значение)
    if (sem_init(&sem, 1, 0) == -1) {
        perror("sem_init failed");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd)) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid < 0) {
        perror("fail fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        Child_Process(pipefd, count);
    }
    else {
        Parent_process(pipefd, count);
    }

    // Уничтожаем семафор после завершения работы
    sem_destroy(&sem);

    return 0;
}
