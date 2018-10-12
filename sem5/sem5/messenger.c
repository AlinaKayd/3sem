//
//  messenger.c
//  sem5
//
//  Created by mac on 11.10.2018.
//  Copyright © 2018 mac. All rights reserved.
//
#define maxstr 1024
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

int main(int argc, char **argv){
    
    int fd1, fd2, result;
    char name1[]="1.fifo";
    char name2[]="2.fifo";
    char * str1 = (char*)malloc(maxstr*sizeof(char));
    char * str2 = (char*)malloc(maxstr*sizeof(char));
    //(void)umask(0);
    if(mknod(name1, S_IFIFO | 0666, 0) < 0){
        if(errno != EEXIST){
            printf("error");
            exit(-1);
        }
    }
    if(mknod(name2, S_IFIFO | 0666, 0) < 0){
        if(errno != EEXIST){
            printf("error");
            exit(-1);
        }
    }
    result = fork();
    if(result == 0)
    {
        printf("child\n");
    }
    if(result > 0)
    {
        printf("parent\n");
    }
    if(result < 0){
        printf("Can\'t fork child\n");
        exit(-1);
    }
    if(result > 0) {
        if(atoi(argv[1]) == 2){
            if((fd2 = open(name2, O_WRONLY)) < 0){
                printf("Can\'t open FIFO for writting\n");
                exit(-1);
            }
            while(1)
            {
                fgets(str1, maxstr, stdin);
                write(fd2, stdin, strlen(str1));
            }
        }
        if(atoi(argv[1]) == 1){
            if((fd1 = open(name1, O_WRONLY)) < 0){
                printf("Can\'t open FIFO for writting\n");
                exit(-1);
            }
            while(1)
            {
                fgets(str1, maxstr, stdin);
                write(fd1, stdin, strlen(str1));
            }
        }
       // printf("read fd = %d\n", fd);
    }
    if(result == 0) {
        if(atoi(argv[1]) == 2) {
            if((fd1 = open(name1, O_RDONLY)) < 0){
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }
            while(1) {
                read(fd1, str2, maxstr);
                printf("%s\n", str2);
            }
        }
        if(atoi(argv[1]) == 1) {
            if((fd2 = open(name2, O_RDONLY)) < 0){
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
            }
            while(1) {
                read(fd2, str2, maxstr);
                printf("%s\n", str2);
            }
        }
        //printf("write fd = %d\n", fd);
    }
    free(str1);
    free(str2);
    return 0;
}
