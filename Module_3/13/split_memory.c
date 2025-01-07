#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_NUMBERS 100
#define SHM_NAME "/posix_shm_example" 

typedef struct {
    int numbers[MAX_NUMBERS];  
    int min;                   
    int max;                   
    int count;                 
} shm_data_t;

int shm_fd;  
shm_data_t* shm_data;  

void handle_sigint(int sig) {
    printf("\nКоличество обработанных наборов: %d\n", shm_data->count);

    munmap(shm_data, sizeof(shm_data_t));
    shm_unlink(SHM_NAME);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(1);
    }

    ftruncate(shm_fd, sizeof(shm_data_t));

    shm_data = mmap(0, sizeof(shm_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_data == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    srand(time(NULL));
    shm_data->count = 0;

    while (1) {
        int num_count = rand() % MAX_NUMBERS + 1;
        printf("Генерация набора из %d чисел:\n", num_count);

        for (int i = 0; i < num_count; i++) {
            shm_data->numbers[i] = rand() % 1000;
            printf("%d ", shm_data->numbers[i]);
        }
        printf("\n");

        pid_t pid = fork();
        if (pid == 0) {
            int min = shm_data->numbers[0];
            int max = shm_data->numbers[0];

            for (int i = 1; i < num_count; i++) {
                if (shm_data->numbers[i] < min) min = shm_data->numbers[i];
                if (shm_data->numbers[i] > max) max = shm_data->numbers[i];
            }

            shm_data->min = min;
            shm_data->max = max;
            exit(0);
        }
        else if (pid > 0) {
            wait(NULL);

            printf("Минимальное значение: %d\n", shm_data->min);
            printf("Максимальное значение: %d\n", shm_data->max);

            shm_data->count++;
        }
        else {
            perror("fork");
            exit(1);
        }

        sleep(1);  
    }

    return 0;
}
