#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT1 8080  // Порт клиента 1
#define PORT2 9090  // Порт клиента 2
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in my_addr, peer_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка собственного адреса
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(PORT1);

    // Привязка к порту
    if (bind(sockfd, (const struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("Ошибка при привязке");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса собеседника
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(PORT2);
    peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Клиент 1 запущен. Введите сообщение:\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);

        // Отправка сообщения клиенту 2
        sendto(sockfd, buffer, strlen(buffer), 0,
            (struct sockaddr*)&peer_addr, sizeof(peer_addr));

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Выход.\n");
            break;
        }

        sleep(2);  // Задержка, чтобы не получить свое сообщение

        // Получение ответа
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        printf("Ответ: %s", buffer);
    }

    close(sockfd);
    return 0;
}
