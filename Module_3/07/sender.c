#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#define QUEUE_NAME "/my_queue"
#define PRIORITY 1
#define SIZE 256
int main() {
	mqd_t ds;
	char new_text[SIZE];
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = SIZE;
	attr.mq_curmsgs = 0;
	int prio;
	if ((ds = mq_open(QUEUE_NAME,
		O_CREAT | O_RDWR, 0600,
		&attr)) == (mqd_t) -1) {
		perror("Creating queue error");
		return -1;
	}
	while(1) {
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
		if (mq_receive(ds, new_text, SIZE, &prio) == -1) {
			perror("cannot receive");
			return -1;
		}
		printf("Friend: %s\n", new_text);
		if (strcmp(new_text, "exit") == 0) {
			break;
		}
		
	}
	if (mq_close(ds) == -1)
		perror("Closing queue error");
	return 0;
	
}