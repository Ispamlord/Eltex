#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Создание UDP сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    printf("Отправитель готов. Введите сообщение:\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);  // Чтение строки из терминала

        // Отправка сообщения серверу
        sendto(sockfd, buffer, strlen(buffer), 0,
            (struct sockaddr*)&server_addr, addr_len);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Чат завершен.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
