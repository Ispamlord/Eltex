#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT1 8080
#define PORT2 9090
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in peer_addr;
socklen_t addr_len = sizeof(peer_addr);

// Поток для приема сообщений
void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&peer_addr, &addr_len);
        printf("\nОтвет: %s", buffer);
    }
}

int main() {
    struct sockaddr_in my_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса для приема
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = INADDR_ANY;
    my_addr.sin_port = htons(PORT2);

    // Привязка сокета
    if (bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) < 0) {
        perror("Ошибка при привязке");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса собеседника
    memset(&peer_addr, 0, sizeof(peer_addr));
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(PORT1);
    peer_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Поток для приема сообщений
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);

    printf("Клиент 2 запущен. Введите сообщение:\n");

    // Основной цикл отправки
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&peer_addr, addr_len);
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Выход.\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
