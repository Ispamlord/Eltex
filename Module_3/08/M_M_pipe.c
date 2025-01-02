﻿#include <sys/types.h>
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
sem_t read_sem;
sem_t write_sem;

void Parent_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sem_post(&write_sem);

        // Читаем из канала
        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Read from pipe: %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        sem_wait(&read_sem);

        sleep(1);
    }

    close(pipefd[0]);
    wait(NULL);
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);
    for (int i = 0; i < count; i++) {
        sem_wait(&write_sem);  // Ожидание разрешения от родителя

        int r = rand() % 100;
        printf("Write to pipe: %d\n", r);

        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            exit(1);
        }
        sem_post(&read_sem);

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

    // Инициализация семафора с 1 (разрешить первую запись)
    sem_init(&write_sem, 1, 1);  // Запись доступна
    sem_init(&read_sem, 1, 1);

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

    sem_destroy(&sem);  // Уничтожаем семафор после работы
    return 0;
}