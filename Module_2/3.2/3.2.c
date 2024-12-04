#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_IP 4294967295  // Максимальное значение для 32-битного IP-адреса

// Функция для преобразования IP в число
unsigned int ip_to_int(const char* ip) {
    struct in_addr addr;
    inet_pton(AF_INET, ip, &addr);
    return ntohl(addr.s_addr);
}

// Функция для преобразования числа в IP-адрес
void int_to_ip(unsigned int ip_int, char* ip) {
    struct in_addr addr;
    addr.s_addr = htonl(ip_int);
    inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
}

// Функция для проверки принадлежности IP-адреса подсети
bool is_in_subnet(unsigned int ip, unsigned int subnet, unsigned int mask) {
    return (ip & mask) == (subnet & mask);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Использование: %s <IP-адрес шлюза> <Маска подсети> <Количество пакетов>\n", argv[0]);
        return 1;
    }

    // Преобразуем входные аргументы
    char gateway_ip[INET_ADDRSTRLEN];
    char subnet_mask[INET_ADDRSTRLEN];
    int num_packets;

    strcpy(gateway_ip, argv[1]);
    strcpy(subnet_mask, argv[2]);
    num_packets = atoi(argv[3]);

    // Преобразуем IP-адрес шлюза и маску подсети в числа
    unsigned int gateway = ip_to_int(gateway_ip);
    unsigned int mask = ip_to_int(subnet_mask);

    // Определяем сеть, к которой принадлежит шлюз
    unsigned int subnet = gateway & mask;

    // Статистика
    int same_network_count = 0;
    int other_network_count = 0;

    srand(time(NULL));

    // Генерация и обработка пакетов
    for (int i = 0; i < num_packets; i++) {
        // Генерация случайного IP-адреса назначения
        unsigned int destination_ip = rand() % MAX_IP;

        // Проверяем, принадлежит ли адрес подсети
        if (is_in_subnet(destination_ip, subnet, mask)) {
            same_network_count++;
        }
        else {
            other_network_count++;
        }
    }
    
    printf("Количество пакетов, предназначенных для узлов своей подсети: %d (%.2f%%)\n",
        same_network_count, (float)same_network_count / num_packets * 100);
    printf("Количество пакетов, предназначенных для узлов других сетей: %d (%.2f%%)\n",
        other_network_count, (float)other_network_count / num_packets * 100);

    return 0;
}
