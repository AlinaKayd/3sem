//
//  main.c
//  hw threads
//
//  Created by mac on 01.11.2018.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 1000

int res [MAX_SIZE][MAX_SIZE];
int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int N;
pthread_t thid[MAX_SIZE];

void * Func(void *dummy)
{
    pthread_t mythid;
    mythid = pthread_self();
    int row;
    // Вот здесь вот вы используете предположение, что число потоков ровно совпадает с числом строк.
    // Но по заданию число потоков K - это отдельный входной параметр
    // K стоит варьировать от 1 до числа процессоров на компьютере (до 4 или 8).
    // Также стоит удостовериться, что при одних и тех же матрицах на входе, но разном K получаете один и тот же результат перемножения
    for(row = 0; (row < N )&(mythid != thid[row]); row++)
    {
    }
    for(int col = 0; col < N; col++)
    {
        res[row][col] = 0;
        for(int c = 0; c < N; c++)
        {
            res [row][col] += a[row][c] * b[c][col];
        }
    }
    return NULL;
}

int main() {
    printf("enter the size of matrix\n");
    scanf("%d", &N);
    int result;
    
    /*
    FIXIT:
    вы сможете увидеть ускорение только на больших матрицах.
    дествительно смогли вбить матрицу 1000*1000 вручную?)
    */
    
    for(int rowa = 0; rowa < N; rowa++)
    {
        printf("enter the %d row of matrix a\n", rowa);
        for(int cola = 0; cola < N; cola++)
        {
            scanf("%d", &a[rowa][cola]);
        }
    }
    
    for(int rowb = 0; rowb < N; rowb++)
    {
        printf("enter the %d row of matrix b\n", rowb);
        for(int colb = 0; colb < N; colb++)
        {
            scanf("%d", &b[rowb][colb]);
        }
    }
    
    // FIXIT: у вас число нитей K должно задаваться дополнительно ... оно не обязательно равно N
    for(int i = 0; i < N; i++)
    {
        result = pthread_create( &thid[i], (pthread_attr_t *)NULL, Func, NULL);
        
        if(result != 0){
            printf ("Error on thread 1 create, return value = %d\n", result);
            exit(-1);
        }
    }
    
    for(int i = 0; i < N; i++)
    {
        pthread_join(thid[i], (void **)NULL);
    }
    printf("the result is\n");
    
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            printf("%4d ", res[i][j]);
        }
        printf("\n");
    }
    return 0;
}
