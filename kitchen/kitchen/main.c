//
//  main.c
//  kitchen
//
//  Created by mac on 22/11/2018.
//  Copyright © 2018 mac. All rights reserved.
//

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define TABLE_SIZE 1
#define MAX_NAME_SIZE 10
#define MY_SEM_PUSH( _SEM_NUM_, _OP_, _FLAGS_, _COM_NUM_) \
if(2+2 == 4)                                            \
{                                                       \
mybuf[_COM_NUM_].sem_op = _OP_;                         \
mybuf[_COM_NUM_].sem_flg = _FLAGS_;                     \
mybuf[_COM_NUM_].sem_num = _SEM_NUM_;                   \
}


typedef struct Stack {
    char data[TABLE_SIZE][MAX_NAME_SIZE];
    int size;
}stack_type;

stack_type * table;

struct p {
    char * name;
    int washtime;
    int wipetime;
};
struct d {
    char * name;
    int number;
};

void push(char * name)
{
    if(table->size == TABLE_SIZE) {
        printf("WOW! TABLE IS BUSY!\n");
        exit(-1);
    }
    strcpy(table->data[table->size], name);
    table->size++;
}

int pop(struct p * properties) {
    int i;
    if (table->size == 0) {
        printf("WOW! TABLE IS EMPTY!\n");
        exit(-1);
    }
    table->size--;
    for(i = 0; strcmp(table->data[table->size], properties[i].name); i++);
    return i;
}

void Pushdish(int semid, char * name)
{
    struct sembuf mybuf[10];
    MY_SEM_PUSH( 0, -1, 0, 0);
    MY_SEM_PUSH( 1, 1, 0, 1);
    MY_SEM_PUSH( 2, 0, 0, 2); // wait(s2 == 0)
    MY_SEM_PUSH( 2, 1, 0, 3); // s2++;
    if(semop(semid, mybuf, 4) < 0){
        printf("SEMAPHORES ERROR\n");
        exit(-1);
    }
    push(name);
    
    MY_SEM_PUSH(2, -1, 0, 0);
    semop(semid, mybuf, 1); // s2--;
}

int Getdish(int semid, struct p * properties)
{
    struct sembuf mybuf[10];
    MY_SEM_PUSH( 0, 1, 0, 0);
    MY_SEM_PUSH( 1, -1, 0, 1);
    MY_SEM_PUSH( 2, 0, 0, 2);
    MY_SEM_PUSH( 2, 1, 0, 3);
    if(semop(semid, mybuf, 4) < 0){
        printf("SEMAPHORES ERROR\n");
        exit(-1);
    }
    int ret = pop(properties);
    
    MY_SEM_PUSH(2, -1, 0, 0);
    semop(semid, mybuf, 1);
    
    return ret;
}

void Wash(struct d dishes, struct p * properties, int semid)
{
    int i;
    for(i = 0; strcmp(dishes.name, properties[i].name); i++);
    for(int j = 0; j < dishes.number; j++)
    {
        sleep(properties[i].washtime);
        printf("%s washed...", properties[i].name);
        Pushdish(semid, properties[i].name);
        printf(" and pushed!\n");
    }
}

void Wipe(int semid, struct p * properties)
{
    int i;
    i = Getdish(semid, properties);
    printf("%s got...", properties[i].name);
    sleep(properties[i].wipetime);
    printf(" and wiped!\n");
}


int main(int argc, const char * argv[])
{
    struct sembuf mybuf[10];
    int N;
    key_t key1, key2;
    int shmid;
    int semid;
   
    struct p properties[10];
    struct d dirtydishes[10];
    

    char pathname1[] = "1.c";
    char pathname2[] = "2.c";
    
    //enum{ one, two, amount }
    //sem_com.sem_id[i] = 1;
    //create semaphore
    if((key2 = ftok(pathname2,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((semid = semget(key2, 3, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    printf("SEMID IS %d\n", semid);
    
    //create shared memory
    if((key1 = ftok(pathname1,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((shmid = shmget(key1, sizeof(stack_type), 0666|IPC_CREAT|IPC_EXCL)) < 0){
        
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");
            exit(-1);
        } else {
            if((shmid = shmget(key1, sizeof(stack_type), 0)) < 0){
                printf("Can\'t find shared memory\n");
                exit(-1);
            }
        }
    }
    if((table = (stack_type *)shmat(shmid, NULL, 0)) == NULL){
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    table->size = 0;
    
    
    properties[0].name = "cup";   properties[0].washtime = 1; properties[0].wipetime = 5;
    properties[1].name = "spoon"; properties[1].washtime = 3; properties[1].wipetime = 2;
    properties[2].name = "board"; properties[2].washtime = 3; properties[2].wipetime = 2;
    properties[3].name = "plate"; properties[3].washtime = 2; properties[3].wipetime = 5;
    properties[4].name = "fork";  properties[4].washtime = 3; properties[4].wipetime = 2;
    
    
    dirtydishes[0].name = "cup"; dirtydishes[0].number = 3;
    dirtydishes[1].name = "plate"; dirtydishes[1].number = 7;
    N = 2;
    
    printf("PROPERTIES\n");
    for(int i = 0; i < 5; i++)
    {
        printf("%s %d %d\n", properties[i].name, properties[i].washtime, properties[i].wipetime);
    }
    
    printf("DISHES\n");
    for(int i = 0; i < 2; i++)
    {
        printf("%s %d\n", dirtydishes[i].name, dirtydishes[i].number);
    }
    
    printf("TABLE SIZE IS %d\n", TABLE_SIZE);
    
    
    if(atoi(argv[1]) == 2)
    {
        printf("WIPE\n");
        MY_SEM_PUSH(0, TABLE_SIZE, 0, 0);
        if(semop(semid, mybuf, 1) < 0){
            printf("SEMAPHORES ERROR\n");
            exit(-1);
        }
        while(1)
        {
            Wipe(semid, properties);
        }
    }
    
    if(atoi(argv[1]) == 1)
    {
        printf("WASH\n");
        for(int i = 0; i < N; i++)
        {
            Wash(dirtydishes[i], properties, semid);
        }
    }
    return 0;
}
