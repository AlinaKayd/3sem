//
//  system.c
//  useless
//
//  Created by mac on 05.10.2018.
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

int main()
{
    char str[1024];
    fgets(str, 1024, stdin);
    system(str);
    return 0;
}
