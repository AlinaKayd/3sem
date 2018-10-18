//
//  main.c
//  useless
//
//  Created by mac on 04.10.2018.
//  Copyright © 2018 mac. All rights reserved.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <signal.h>

char** Split(char* string, char* delimiters, int* nwords)
{
    char** arrwords = 0;
    int c = 0;
    char* pch = strtok(string, delimiters);
    while (pch != NULL)
    {
        arrwords = (char**)realloc(arrwords, sizeof(char*) * ++c);
        arrwords[c - 1] = pch;
        pch = strtok(NULL, delimiters);
    }
    *nwords = c;
    return arrwords;
}

int main()
{
    char str[1024];
    int n;
    scanf("%d\n", &n);
    for(int i = 0; i < n; i++)
    {
        fgets(str, 1024, stdin);
        system(str);
    }
    return 0;
}
