#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>

#define MAX_NUMBERS 10
#define SHM_SIZE (MAX_NUMBERS * sizeof(int) + 2 * sizeof(int)) 

typedef struct {
    int numbers[MAX_NUMBERS];  
    int min;                   
    int max;                  
} shm_data_t;

int shmid;  

void handle_sigint(int sig) {
    shm_data_t* shm_data = (shm_data_t*)shmat(shmid, NULL, 0);
    if (shm_data == (shm_data_t*)-1) {
        perror("shmat");
        exit(1);
    }

    printf("\nКоличество обработанных наборов: %d\n", shm_data->max);
    shmdt(shm_data); 
    shmctl(shmid, IPC_RMID, NULL);  
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    shmid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm_data_t* shm_data = (shm_data_t*)shmat(shmid, NULL, 0);
    if (shm_data == (shm_data_t*)-1) {
        perror("shmat");
        exit(1);
    }

    srand(time(NULL)); 

    while (1) {
        int num_count = rand() % MAX_NUMBERS + 1; 
        printf("Генерация набора из %d чисел:\n", num_count);

        for (int i = 0; i < num_count; i++) {
            shm_data->numbers[i] = rand() % 1000; 
            printf("%d ", shm_data->numbers[i]);
        }

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

            printf("\nМинимальное значение: %d\n", shm_data->min);
            printf("Максимальное значение: %d\n", shm_data->max);
        }
        else {
            perror("fork");
            exit(1);
        }

        sleep(1); 
    }

    return 0;
}
