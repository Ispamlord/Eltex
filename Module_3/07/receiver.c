#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>
//reciver.c
#define QUEUE_NAME "/chat_queue"
#define MAX_SIZE 1024
#define END_PRIORITY 10

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];
    struct mq_attr attr;
    unsigned int prio;
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0644, &attr);
    if (mq == -1) {
        perror("mq_open");
        exit(1);
    }

    printf("Waiting for messages...\n");
    while (1) {
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, &prio);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';
            printf("Friend: %s\n", buffer);
        }

        if (prio == END_PRIORITY) {
            printf("Chat ended by the other side.\\n");
            break;
        }
        printf("You: ");
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            mq_send(mq, buffer, strlen(buffer) + 1, END_PRIORITY);
            break;
        }
        mq_send(mq, buffer, strlen(buffer) + 1, 1);
        sleep(1);
    }

    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}
