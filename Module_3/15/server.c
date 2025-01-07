#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void process_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int num1, num2;
    char op;
    double result;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Клиент отключился.\n");
            break;
        }

        sscanf(buffer, "%d %c %d", &num1, &op, &num2);

        switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0)
                result = (double)num1 / num2;
            else {
                strcpy(buffer, "Ошибка: деление на ноль.");
                send(client_socket, buffer, strlen(buffer), 0);
                continue;
            }
            break;
        default:
            strcpy(buffer, "Ошибка: неизвестная операция.");
            send(client_socket, buffer, strlen(buffer), 0);
            continue;
        }

        snprintf(buffer, BUFFER_SIZE, "Результат: %.2f", result);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Ошибка привязки");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Ошибка прослушивания");
        exit(EXIT_FAILURE);
    }

    printf("Ожидание подключения клиента...\n");

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Ошибка подключения");
            exit(EXIT_FAILURE);
        }
        printf("Клиент подключен.\n");

        process_request(client_socket);
    }

    return 0;
}
