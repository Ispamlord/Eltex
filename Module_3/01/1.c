#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>


void print_square(int first,int last, int* args){
    for(int i=first;i<last; i++){
        printf("%d\n",args[i]*args[i]);
    }
}
int main (int argc, char*argv[]){
    if(argc<2){
        printf("Enter side of square");
        return 1;
    }
    int args[argc-1];
    for(int i =1;i<argc;i++){
        args[i-1] = atoi(argv[i]);
    }
    int mid=0;
    if((argc-1)%2==0){
        mid = (argc-1)/2 -1;
    }else{
        mid=(argc-1)/2;
    }
    pid_t p = fork();
    if(p<0){
        perror("fail fork");
        exit(EXIT_FAILURE);
    }
    else if(p == 0){
        print_square(0, mid, args);
    }
    else{
        print_square(mid,argc-1,args);
        wait(NULL);
    }
    
    return 0;
}