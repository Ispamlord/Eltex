#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <semaphore.h>

sem_t read_sem, write_sem;
int reader_count = 0;

void Parent_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sem_wait(&write_sem);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Read from pipe: %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        sem_post(&write_sem);

        sleep(1);
    }

    close(pipefd[0]);
    wait(NULL);
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);

    for (int i = 0; i < count; i++) {
        sem_wait(&write_sem);

        int r = rand() % 100;
        printf("Write to pipe: %d\n", r);

        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            exit(1);
        }

        sem_post(&write_sem);

        sleep(1);
    }
    close(pipefd[1]);
    exit(0);
}
void Reader_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sem_wait(&read_sem);
        reader_count++;
        if (reader_count == 1) {
            sem_wait(&write_sem);
        }
        sem_post(&read_sem);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Reader %d read: %d\n", getpid(), r);
        }
        else {
            perror("read");
            break;
        }

        sem_wait(&read_sem);
        reader_count--;
        if (reader_count == 0) {
            sem_post(&write_sem);
        }
        sem_post(&read_sem);

        sleep(1);
    }

    close(pipefd[0]);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Usage: ./program <count> <num_readers>");
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);
    int num_readers = atoi(argv[2]);

    pid_t pid;
    int pipefd[2];

    sem_init(&write_sem, 1, 1);  
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
    
    for (int i = 0; i < num_readers; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fail fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            Reader_process(pipefd, count);
        }
    }

    for (int i = 0; i < num_readers + 1; i++) {
        wait(NULL);
    }

    sem_destroy(&write_sem);
    sem_destroy(&read_sem);

    return 0;
}
