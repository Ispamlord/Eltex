#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <arpa/inet.h>
uint32_t generate_random_ip() {
    return (uint32_t)rand();
}


int main(int argc, char* argv[]){
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <IP шлюза> <маска подсети> <кол-во пакетов>\n", argv[0]);
        return EXIT_FAILURE;
    }
    const char* gateway_str = argv[1];
    const char* mask_str = argv[2];
    int num_packets = atoi(argv[3]);


    if (num_packets <= 0 || num_packets > MAX_PACKETS) {
        fprintf(stderr, "Ошибка: Количество пакетов должно быть положительным числом (1-%d).\n", MAX_PACKETS);
        return EXIT_FAILURE;
    }
    uint32_t gateway, mask;



    return 0;
}