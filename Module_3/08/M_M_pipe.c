#include <sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>
#include <fcntl.h>
#include<stdbool.h>
#include<signal.h>
volatile sig_atomic_t file_locked = false;
void handle_sigusr1(int sig) {
    file_locked = true;
}
void handle_sigusr2(int sig) {
    file_locked = false;
}

void Parent_process(int* pipefd, int count, pid_t Child_Pid) {
    close(pipefd[1]);

    int r = 0;
    for (int i = 0;i < count;i++) {
        sleep(3);

        kill(Child_Pid, SIGUSR1);

        if (read(pipefd[0], &r, sizeof(r)) > 0) {
            printf("Read from pipe %d\n", r);
        }
        else {
            perror("read");
            break;
        }

        kill(Child_Pid, SIGUSR2);

    }
    close(pipefd[0]);
    wait(NULL);
}
void Child_Process(int* pipefd, int count) {
    signal(SIGUSR1, handle_sigusr1);
    signal(SIGUSR2, handle_sigusr2);
    close(pipefd[0]);
    for (int i = 0; i < count;i++) {
        while (file_locked) {
            usleep(1000);
        }
        int r = rand() % 100;
        printf("Write in pipe: %d\n", r);
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
        perror("Enter 2 element");
        exit(EXIT_FAILURE);
    }

    int count = atoi(argv[1]);

    pid_t pid;
    int r;
    int pipefd[2];
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
    return 0;

}


