#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file(int sock, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return;
    }

    // Сообщаем серверу о передаче файла
    send(sock, "file", 4, 0);
    send(sock, filename, strlen(filename), 0);
    sleep(1);

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(sock, buffer, bytes_read, 0);
    }

    fclose(file);
    printf("Файл %s отправлен серверу.\n", filename);
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = { 0 };

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Неверный адрес");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Ошибка подключения");
        exit(EXIT_FAILURE);
    }

    printf("Подключено к серверу.\n");
    printf("Введите запрос (пример: 10 + 5) или 'file [имя файла]':\n");

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Проверка на команду передачи файла
        if (strncmp(buffer, "file", 4) == 0) {
            char filename[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", filename);
            send_file(sock, filename);
            continue;
        }

        send(sock, buffer, strlen(buffer), 0);

        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Отключение...\n");
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        read(sock, buffer, BUFFER_SIZE);
        printf("%s\n", buffer);
    }

    close(sock);
    return 0;
}
