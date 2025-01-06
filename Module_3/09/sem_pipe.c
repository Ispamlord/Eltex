#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int sem_writer, sem_readers;
int readers_count = 0;

void sem_op(int sem_id, int op) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = op;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

void start_read() {
    sem_op(sem_readers, -1);
    readers_count++;
    if (readers_count == 1) {
        sem_op(sem_writer, -1);
    }
    sem_op(sem_readers, 1);
}

void stop_read() {
    sem_op(sem_readers, -1);
    readers_count--;
    if (readers_count == 0) {
        sem_op(sem_writer, 1);
    }
    sem_op(sem_readers, 1);
}

void Parent_process(int* pipefd, int count, int reader_id) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sleep(3);
        start_read();
        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Reader %d read from pipe: %d\n", reader_id, r);
        }
        else {
            perror("read");
            break;
        }
        stop_read();
    }

    close(pipefd[0]);
    exit(0);
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);

    for (int i = 0; i < count; i++) {
        sleep(1);
        sem_op(sem_writer, -1); // Lock writer

        int r = rand() % 100;
        printf("Child write to pipe: %d\n", r);
        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            sem_op(sem_writer, 1);
            exit(1);
        }

        sem_op(sem_writer, 1);
    }

    close(pipefd[1]);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        perror("Usage: ./program <count> <num_readers>");
        exit(EXIT_FAILURE);
    }
    int num_readers = atoi(argv[2]);
    int count = atoi(argv[1]);
    int pipefd[2];

    key_t key_writer = ftok("/tmp", 'a');
    key_t key_readers = ftok("/tmp", 'b');

    sem_writer = semget(key_writer, 1, 0666 | IPC_CREAT);
    sem_readers = semget(key_readers, 1, 0666 | IPC_CREAT);
    if (sem_writer == -1 || sem_readers == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(sem_writer, 0, SETVAL, 1);
    semctl(sem_readers, 0, SETVAL, 1);

    if (pipe(pipefd)) {
        fprintf(stderr, "Pipe failed.\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        Child_Process(pipefd, count);
    }

    for (int i = 0; i < num_readers; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            Parent_process(pipefd, count, i + 1);  // Pass unique reader ID
        }
    }

    for (int i = 0; i < num_readers + 1; i++) {
        wait(NULL);
    }

    semctl(sem_writer, 0, IPC_RMID);
    semctl(sem_readers, 0, IPC_RMID);

    return 0;
}
