#include <stdio.h>
#include <mqueue.h>
#include<string.h>
#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256
int main() {
	mqd_t ds;
	char new_text[SIZE];
	struct mq_attr attr, old_attr;
	int prio;
	if ((ds = mq_open(QUEUE_NAME,
		O_RDWR | O_NONBLOCK, 0600,
		NULL)) == (mqd_t)-1) {
		perror("Creating queue error");
		return -1;
	}
	while (1) {
		if (mq_receive(ds, new_text, SIZE, &prio) == -1) {
			perror("cannot receive");
			return -1;
		}
		printf("Friend: %s\n", new_text);
		if (strcmp(new_text, "exit") == 0) {
			break;
		}

		printf("You: ");
		fgets(new_text, SIZE, stdin);
		new_text[strcspn(new_text, "\n")] = '\0';
		
		if (mq_send(ds, new_text, strlen(new_text), PRIORITY) == -1) {
			perror("Sending message error");
			return -1;
		}
		
		if (strncmp(new_text, "exit", SIZE) == 0) {
			break;
		}
		
		
	}
	if (mq_close(ds) == -1)
		perror("Closing queue error");
	if (mq_unlink(QUEUE_NAME) == -1)
		perror("Removing queue error");
	return 0;
}