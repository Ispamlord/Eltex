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

int sem_id;

void sem_op(int sem_id, int op) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = op;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

void Parent_process(int* pipefd, int count, pid_t Child_Pid) {
    close(pipefd[1]);
    int r = 0;

    for (int i = 0; i < count; i++) {
        sleep(1);
        sem_op(sem_id, -1);  

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Read from pipe %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        sem_op(sem_id, 1); 
    }

    close(pipefd[0]);
    wait(NULL);
}

void Child_Process(int* pipefd, int count) {
    close(pipefd[0]);

    for (int i = 0; i < count; i++) {
        sem_op(sem_id, -1);  

        int r = rand() % 100;
        printf("Write in pipe: %d\n", r);

        if (write(pipefd[1], &r, sizeof(r)) == -1) {
            perror("write");
            close(pipefd[1]);
            sem_op(sem_id, 1);
            exit(1);
        }

        sem_op(sem_id, 1);  
        sleep(1);
    }

    close(pipefd[1]);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        perror("Enter 2 element");
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);
    pid_t pid;
    int pipefd[2];

    key_t key = ftok("/tmp", 'a');
    sem_id = semget(key, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(sem_id, 0, SETVAL, 1);

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
        Parent_process(pipefd, count, pid);
    }

    semctl(sem_id, 0, IPC_RMID);

    return 0;
}
