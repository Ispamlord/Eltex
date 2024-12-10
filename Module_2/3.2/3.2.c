#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <arpa/inet.h>

int is_in_subnet(uint32_t ip, uint32_t gateway, uint32_t mask) {
    return (ip & mask) == (gateway & mask);
}

uint32_t generate_random_ip() {
    return (rand() % 256) << 24 | (rand() % 256) << 16 | (rand() % 256) << 8 | (rand() % 256);
}

void ip_to_string(uint32_t ip, char* buffer) {
    sprintf(buffer, "%u.%u.%u.%u",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s <gateway_ip> <subnet_mask> <packet_count>\n", argv[0]);
        return 1;
    }

    uint32_t gateway, mask;
    if (!inet_pton(AF_INET, argv[1], &gateway) || !inet_pton(AF_INET, argv[2], &mask)) {
        printf("Invalid IP or subnet mask.\n");
        return 1;
    }
    gateway = ntohl(gateway);
    mask = ntohl(mask);

    int packet_count = atoi(argv[3]);
    if (packet_count <= 0) {
        printf("Invalid packet count.\n");
        return 1;
    }

    srand(time(NULL)); 

    int same_subnet_count = 0;
    int other_subnet_count = 0;

    printf("Processing %d packets...\n", packet_count);

    for (int i = 0; i < packet_count; i++) {
        uint32_t random_ip = generate_random_ip();
        char ip_str[16];
        ip_to_string(random_ip, ip_str);

        if (is_in_subnet(random_ip, gateway, mask)) {
            same_subnet_count++;
            printf("Packet %d: %s is in the same subnet.\n", i + 1, ip_str);
        }
        else {
            other_subnet_count++;
            printf("Packet %d: %s is in a different subnet.\n", i + 1, ip_str);
        }
    }

    printf("\nStatistics:\n");
    printf("Same subnet: %d packets (%.2f%%)\n", same_subnet_count,
        (100.0 * same_subnet_count) / packet_count);
    printf("Different subnet: %d packets (%.2f%%)\n", other_subnet_count,
        (100.0 * other_subnet_count) / packet_count);

    return 0;
}
