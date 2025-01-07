#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // Создание UDP сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса для прослушивания
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязка сокета
    if (bind(sockfd, (const struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(EXIT_FAILURE);
    }

    printf("Ожидание сообщений...\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // Получение сообщения
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&client_addr, &addr_len);
        printf("Получено: %s", buffer);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Чат завершен.\n");
            break;
        }
        fgets(buffer, BUFFER_SIZE, stdin);

        sendto(sockfd, buffer, strlen(buffer), 0,
            (struct sockaddr*)&server_addr, addr_len);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Чат завершен.\n");
            break;
        }
        sleep(1);
    }

    close(sockfd);
    return 0;
}
