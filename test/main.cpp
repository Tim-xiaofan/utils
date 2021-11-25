#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

#define BIN 0
#define STR 1

static void
print_data(uint8_t * data, int size, int type)
{
    int i;

    for(i = 0; i < size; ++i)
    {
        switch(type)
        {
            case BIN:
                printf("%02x", data[i]);
                break;
            case STR:
                printf("%c", data[i]);
                break;
            default:
                fprintf(stderr, "unkown type %d\n", type);
        }
    }
    printf("\n");
}

#define N 1024
static uint8_t bin[N];
static uint8_t str[N * 2];

static void
print_help(const char * program)
{
    fprintf(stderr, "usage : %s hexstr\n", program);
}

int main(int ac, char * av[])
{
    int size;

    if(ac < 2)
    {
        print_help(av[0]);
        return -1;
    }

    size = hexstr2bin((uint8_t *)av[1], 
                strlen(av[1]), bin, N);
    print_data(bin, size, BIN);
    
    size = bin2hexstr(bin, size, str, N * 2);
    print_data(str, size, STR);
    return 0;
}
