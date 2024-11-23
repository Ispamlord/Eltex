#include<stdio.h>
#include<stdlib.h>
#include <sys/stat.h>
#include<string.h>
#include <unistd.h>


void print_binar(mode_t mode) {
	printf("Бинарное представление");
	printf((mode & S_IRUSR) ? "r" : "-");
	printf((mode & S_IWUSR) ? "w" : "-");
	printf((mode & S_IXUSR) ? "x" : "-");
	printf((mode & S_IRGRP) ? "r" : "-");
	printf((mode & S_IWGRP) ? "w" : "-");
	printf((mode & S_IXGRP) ? "x" : "-");
	printf((mode & S_IROTH) ? "r" : "-");
	printf((mode & S_IWOTH) ? "w" : "-");
	printf((mode & S_IXOTH) ? "x" : "-");
	printf("Цифровое представление: %o\n", mode & 0777);
}
void get_file_stat(const char* filename) {
	struct stat filestat;

	if (stat(filename, &filestat) < 0) {
		perror("Error! Cant be information about file");
		return;
	}
	print_binar(filestat.st_mode);
}
mode_t parse(const char perm) {
	mode_t mode = 0;

	if (strlen(perm) != 9) {
		fprintf(stderr, "Неверный формат строки прав доступа. Должно быть 9 символов (например, rwxr-xr--).\n");
		exit(EXIT_FAILURE);
	}
	if (perm[0] == 'r') mode |= S_IRUSR;
	if (perm[1] == 'w') mode |= S_IWUSR;
	if (perm[2] == 'x') mode |= S_IXUSR;
	if (perm[3] == 'r') mode |= S_IRGRP;
	if (perm[4] == 'w') mode |= S_IWGRP;
	if (perm[5] == 'x') mode |= S_IXGRP;
	if (perm[6] == 'r') mode |= S_IROTH;
	if (perm[7] == 'w') mode |= S_IWOTH;
	if (perm[8] == 'x') mode |= S_IXOTH;
	return mode;

}
mode_t parse_permission_string(const char* perm_str) {
	mode_t mode = 0;

	if (strlen(perm_str) != 9) {
		fprintf(stderr, "Неверный формат строки прав доступа. Должно быть 9 символов (например, rwxr-xr--).\n");
		exit(EXIT_FAILURE);
	}

	if (perm_str[0] == 'r') mode |= S_IRUSR;
	if (perm_str[1] == 'w') mode |= S_IWUSR;
	if (perm_str[2] == 'x') mode |= S_IXUSR;

	if (perm_str[3] == 'r') mode |= S_IRGRP;
	if (perm_str[4] == 'w') mode |= S_IWGRP;
	if (perm_str[5] == 'x') mode |= S_IXGRP;

	if (perm_str[6] == 'r') mode |= S_IROTH;
	if (perm_str[7] == 'w') mode |= S_IWOTH;
	if (perm_str[8] == 'x') mode |= S_IXOTH;

	return mode;
}
int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Использование:\n");
		fprintf(stderr, "%s <файл> - отобразить права доступа файла\n", argv[0]);
		fprintf(stderr, "%s -p <права> - отобразить права доступа из строки\n", argv[0]);
		return EXIT_FAILURE;
	}
	

	if (strcmp(argv[1], "-p") == 0) {
		if (argc != 3) {
			fprintf(stderr, "Ошибка: укажите строку прав доступа (например, rwxr-xr-- или 755).\n");
			return EXIT_FAILURE;
		}

		mode_t mode;
		if (strlen(argv[2]) == 3) {
			mode = strtol(argv[2], NULL, 8);
		}
		else {
			mode = parse_permission_string(argv[2]);
		}

		print_binar(mode);
	}
	else {
		parse(argv[1]);
	}
	
}