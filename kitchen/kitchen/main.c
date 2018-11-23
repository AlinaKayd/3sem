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
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#define MAX_STACK_SIZE 50

typedef struct Stack {
    char * data[MAX_STACK_SIZE];
    size_t size;
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

void push(char * name) {
    if (table->size >= MAX_STACK_SIZE) {
        exit(-1);
    }
    table->data[table->size] = name;
    table->size++;
}
char * pop() {
    if (table->size == 0) {
        exit(-1);
    }
    table->size--;
    return table->data[table->size];
}
void Pushdish(int semid, char * name)
{
    struct sembuf mybuf;
    
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 1;
    semop(semid, &mybuf, 1);
    push(name);
}
char * Getdish(int semid)
{
    struct sembuf mybuf;
    
    mybuf.sem_op = -1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 1;
    semop(semid, &mybuf, 1);
    return pop();
}

void Wash(struct d dishes, struct p * properties, int semid)
{
    int i;
    for(i = 0; strcmp(dishes.name, properties[i].name); i++);
    for(int j = 0; j < dishes.number; j++)
    {
        sleep(properties[i].washtime);
        printf("%s washed\n", properties[i].name);
        Pushdish(semid, properties[i].name);
        printf("%s pushed\n", properties[i].name);
    }
}

void Wipe(int semid, struct p * properties)
{
    char * name;
    int i;
    name = Getdish(semid);
    printf("%s getted\n", name);
    for(i = 0; strcmp(name, properties[i].name); i++);
    sleep(properties[i].wipetime);
    printf("%s wiped\n", name);
}


int main(int argc, const char * argv[])
{
   key_t   key1, key2;
    int shmid;
    int semid;
    int nprop = 0;
    int nstrdishes = 0;
    struct p properties[10];
    struct d dirtydishes[10];
    
    FILE * property;
    FILE * dirtydish;
    char pathname1[] = "1.c";
    char pathname2[] = "2.c";
    
    //create semaphore
    if((key2 = ftok(pathname2,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((semid = semget(key2, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    
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
    
    property = fopen("properties.txt", "r");
    while(fscanf(property, "%s%d%d", properties[nprop].name, &(properties[nprop].washtime), &(properties[nprop].wipetime)) != EOF) {
        printf("%s wash %d sec  wipe %d sec\n", properties[nprop].name, properties[nprop].washtime, properties[nprop].wipetime);
        nprop++;
    }
    fclose(property);
    /*
    dirtydish = fopen("dirtydishes.txt", "r");
    while(fscanf(dirtydish, "%s%d", dirtydishes[nstrdishes].name, &(dirtydishes[nstrdishes].number)) != EOF) {
        printf("%s's number is %d\n", dirtydishes[nstrdishes].name, dirtydishes[nstrdishes].number);
        nstrdishes++;
    }
    fclose(dirtydish);
    
    if(atoi(argv[1]) == 1)
    {
        for(int i = 0; i <= nstrdishes; i++)
        {
            Wash(dirtydishes[i], properties, semid);
        }
    }
    if(atoi(argv[1]) == 2)
    {
        while(1)
        {
            Wipe(semid, properties);
        }
    }
    
    if(shmdt(properties) < 0){
        printf("Can't detach shared memory\n");
        exit(-1);
    }*/
    return 0;
}
