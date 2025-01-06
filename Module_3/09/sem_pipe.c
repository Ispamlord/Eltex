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

int write_sem, read_sem;
int reader_count = 0;
int shm_id;
int* shared_count;

void sem_op(int sem_id, int op) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = op;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

void Parent_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sem_op(write_sem, -1);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Parent read from pipe: %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        sem_op(write_sem, 1);
        sleep(1);
    }

    close(pipefd[0]);
    wait(NULL);
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);

    for (int i = 0; i < count; i++) {
        sem_op(write_sem, -1);

        int r = rand() % 100;
        printf("Write to pipe: %d\n", r);

        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            exit(1);
        }

        sem_op(write_sem, 1);
        sleep(1);
    }
    close(pipefd[1]);
    exit(0);
}

void Reader_process(int* pipefd, int count) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sem_op(read_sem, -1);
        (*shared_count)++;
        if (*shared_count == 1) {
            sem_op(write_sem, -1);
        }
        sem_op(read_sem, 1);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Reader %d read: %d\n", getpid(), r);
        }
        else {
            perror("read");
            break;
        }

        sem_op(read_sem, -1);
        (*shared_count)--;
        if (*shared_count == 0) {
            sem_op(write_sem, 1);
        }
        sem_op(read_sem, 1);

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
    int pipefd[2];
    pid_t pid;

    key_t key1 = ftok("/tmp", 'a');
    key_t key2 = ftok("/tmp", 'b');

    write_sem = semget(key1, 1, 0666 | IPC_CREAT);
    read_sem = semget(key2, 1, 0666 | IPC_CREAT);
    shm_id = smget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT);

    if (write_sem == -1 || read_sem == -1 || shm_id == -1) {
        perror("semget/shmget");
        exit(EXIT_FAILURE);
    }

    semctl(write_sem, 0, SETVAL, 1);
    semctl(read_sem, 0, SETVAL, 1);
    shared_count = (int*)shmat(shm_id, NULL, 0);
    *shared_count = 0;

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

    semctl(write_sem, 0, IPC_RMID);
    semctl(read_sem, 0, IPC_RMID);
    shmdt(shared_count);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
