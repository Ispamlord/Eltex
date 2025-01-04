#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define LEN 128
int main() {
	char buf[LEN];
	memset(buf,0,LEN);
	int count = 0;
	puts("Введите строку для записывания в chardev");
	if (fgets(buf, LEN, stdin) != NULL) {
		if (buf[strlen(buf) - 1] == "\n") {
			buf[strlen(buf) - 1] = "\0";
		}
	}
	int fd = open("/dev/chardev",O_RDWR);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	if ((count = write(fd, buf, strlen(buf) + 1)) < 0) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	printf("Количество записаных байт в chardev: %d\n", count);
	if ((count = read(fd, buf, LEN)) < 0) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("Количество прочитанных байт в chardev: %d\n", count);
	printf("Вывод: %s\n", buf);
	exit(EXIT_SUCCESS);

}