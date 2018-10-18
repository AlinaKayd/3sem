//
//  main.c
//  abacaba
//
//  Created by mac on 04.10.2018.
//  Copyright © 2018 mac. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void Generate_string(char * x, int n)
{
    char i = 0;
    x[0] = 'a';
   printf("строка%d %s", i, x);
    for(i = 0; i < n; i++)
    {
        x[strlen(x)] = 'a' + i + 1;
        strncat(x, x, strlen(x) - 1);
    }
    //x[strlen(x) + 1] = '\0';
}
int main()
{
    char * x;
    int n = 2;
    //scanf("%d", &n);
    n -= 1;
    x = malloc((1<<(n + 1))*sizeof(char));
    Generate_string(x, n);
    printf("%s", x);
    free(x);
    return 0;
}
