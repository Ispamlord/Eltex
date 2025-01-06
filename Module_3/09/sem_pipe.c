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
#include <sys/shm.h>

#define SHM_KEY 1234

int mutex, write_sem, shm_id;
int* read_count;

void sem_op(int sem_id, int op) {
    struct sembuf sem_buf;
    sem_buf.sem_num = 0;
    sem_buf.sem_op = op;
    sem_buf.sem_flg = 0;
    semop(sem_id, &sem_buf, 1);
}

void Reader_Process(const char* filename) {
    int fd;
    char buffer[100];

    for (int i = 0; i < 5; i++) {
        sem_op(mutex, -1);
        (*read_count)++;
        if (*read_count == 1) {
            sem_op(write_sem, -1);  
        }
        sem_op(mutex, 1);

        // Read section
        fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("File open (reader)");
        }
        else {
            printf("Reader [%d]: Reading...\n", getpid());
            read(fd, buffer, sizeof(buffer));
            printf("Reader [%d]: %s\n", getpid(), buffer);
            close(fd);
        }

        sem_op(mutex, -1);
        (*read_count)--;
        if (*read_count == 0) {
            sem_op(write_sem, 1); 
        }
        sem_op(mutex, 1);

        sleep(1);
    }
}

void Writer_Process(const char* filename) {
    int fd;
    const char* message = "Writing to file by writer process.\n";

    for (int i = 0; i < 5; i++) {
        sem_op(write_sem, -1); 

        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd == -1) {
            perror("File open (writer)");
        }
        else {
            printf("Writer [%d]: Writing...\n", getpid());
            write(fd, message, strlen(message));
            close(fd);
        }

        sem_op(write_sem, 1);  
        sleep(2);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shm_id = shmget(SHM_KEY, sizeof(int), 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    read_count = (int*)shmat(shm_id, NULL, 0);
    *read_count = 0;

    key_t key1 = ftok("/tmp", 'a');
    key_t key2 = ftok("/tmp", 'b');

    mutex = semget(key1, 1, 0666 | IPC_CREAT);
    write_sem = semget(key2, 1, 0666 | IPC_CREAT);

    if (mutex == -1 || write_sem == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    semctl(mutex, 0, SETVAL, 1);     
    semctl(write_sem, 0, SETVAL, 1); 

    pid_t pid;

    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid == 0) {
            Reader_Process(argv[1]);
            exit(0);
        }
    }

    pid = fork();
    if (pid == 0) {
        Writer_Process(argv[1]);
        exit(0);
    }
    while (wait(NULL) > 0);

    semctl(mutex, 0, IPC_RMID);
    semctl(write_sem, 0, IPC_RMID);
    shmdt(read_count);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
