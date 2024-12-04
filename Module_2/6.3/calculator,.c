#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

typedef double (*operation_func)(int, ...);

typedef struct {
    char* name;
    operation_func func;
} Operation;

Operation* load_operations(const char* dir_path, int* count) {
    DIR* dir = opendir(dir_path);
    if (!dir) {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    struct dirent* entry;
    int capacity = 4;
    Operation* operations = malloc(capacity * sizeof(Operation));
    *count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".so")) {
            if (*count == capacity) {
                capacity *= 2;
                operations = realloc(operations, capacity * sizeof(Operation));
            }

            char lib_path[256];
            snprintf(lib_path, sizeof(lib_path), "%s/%s", dir_path, entry->d_name);

            void* handle = dlopen(lib_path, RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Failed to load library: %s\n", dlerror());
                continue;
            }

            operation_func func = (operation_func)dlsym(handle, "sum");
            if (!func) {
                fprintf(stderr, "Failed to load function: %s\n", dlerror());
                dlclose(handle);
                continue;
            }

            operations[*count].name = strdup(entry->d_name);
            operations[*count].func = func;
            (*count)++;
        }
    }

    closedir(dir);
    return operations;
}

int main() {
    const char* lib_dir = "./libs";
    int op_count = 0;
    Operation* operations = load_operations(lib_dir, &op_count);

    if (op_count == 0) {
        printf("No operations loaded. Exiting.\n");
        return 1;
    }

    printf("Loaded operations:\n");
    for (int i = 0; i < op_count; i++) {
        printf("%d: %s\n", i + 1, operations[i].name);
    }

    while (1) {
        printf("Choose an operation (1-%d) or 0 to exit: ", op_count);
        int choice;
        scanf("%d", &choice);

        if (choice == 0) break;
        if (choice < 1 || choice > op_count) {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        printf("Enter the number of arguments: ");
        int num_args;
        scanf("%d", &num_args);

        if (num_args < 2) {
            printf("At least two arguments are required.\n");
            continue;
        }

        double* args = malloc(num_args * sizeof(double));
        for (int i = 0; i < num_args; i++) {
            printf("Enter argument %d: ", i + 1);
            scanf("%lf", &args[i]);
        }

        double result = operations[choice - 1].func(num_args, args[0], args[1], args[2], args[3]);
        printf("Result: %lf\n", result);

        free(args);
    }

    for (int i = 0; i < op_count; i++) {
        free(operations[i].name);
    }
    free(operations);

    return 0;
}
