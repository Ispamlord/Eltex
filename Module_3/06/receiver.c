#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_TEXT 100
#define END_PRIORITY 99

typedef struct {
	long mtype;
	char Data[MAX_TEXT];
} Message;


int main(int argc, char*argv[]) {
	key_t key; int msgid;
	Message mes;
	key = ftok("text.txt",65);
	if (key == -1) {
		perror("Key");
		exit(EXIT_FAILURE);
	}
	msgid = msgget(key, 0666 | IPC_CREAT);
	for (;;) {
		if (msgrcv(msgid, &mes, sizeof(mes.Data), 1, 0) == -1) {
			perror("msgrcv");
			exit(EXIT_FAILURE);
		}
		printf("Получено: %s\n", mes.Data);
		if (mes.mtype == END_PRIORITY || strcmp(mes.Data, "exit") == 0) {
			printf("Получено завершение обмена. Receiver завершает работу.\n");
			break;
		}
		printf("You: ");
		fgets(mes.Data, MAX_TEXT, stdin);
		mes.Data[strcspn(mes.Data, "\n")] = '\0';
		mes.mtype = 2;
		if (msgsnd(msgid, &mes, sizeof(mes.Data), 0) == -1) {
			perror("msgsnd");
			exit(EXIT_FAILURE);
		}
		if (strcmp("exit", mes.Data) == 0) {
			mes.mtype = END_PRIORITY;
			msgsnd(msgid, &mes, sizeof(mes.Data), 0);
			break;
		}
	}
	printf("Receiver завершил работу.\n");
}