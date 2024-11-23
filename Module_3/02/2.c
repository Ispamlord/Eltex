#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
#define MAX_ARGS 100
int main(){
    char* enter[MAX_ARGS];
    char cmd[1024];
    char* split;

    while(1)
    {
        printf("Введите программу: ");
        if(fgets(cmd,sizeof(cmd), stdin) == NULL){
            printf("Ошмбка при вводе");
            break;
        }
        if(strcmp("exit\n",cmd)==0){
            break;
        }
        split = strtok(cmd, " \n");
        int i = 0;
        while (split !=NULL&&i<MAX_ARGS-1)
        {
            enter[i++] = split;
            split = strtok(NULL, " \n");
        }
        enter[i] = NULL;
        
        pid_t pid = fork();
        if(pid<0){
            perror("fail fork");
            exit(EXIT_FAILURE);
        }
        else if(pid == 0){
            if(execvp(enter[0], enter) == -1){
                perror("Ошибка exec");
            }
            exit(EXIT_FAILURE);
        }
        else{
            int status;
            waitpid(pid, &status, 0);
        }
    }
    return 0;
}
