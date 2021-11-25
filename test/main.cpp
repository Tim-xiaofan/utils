#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

static void
print_bin(uint8_t * bin, int size)
{
    int i;

    for(i = 0; i < size; ++i)
    {
        printf("%02x", bin[i]);
    }
    printf("\n");
}

static uint8_t bin[1024];

static void
print_help(const char * program)
{
    fprintf(stderr, "usage : %s hexstr\n", program);
}

static int 
hexstr2bin(void)
{
    return 0;
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
                strlen(av[1]), bin, 1024);
    print_bin(bin, size);
    hexstr2bin();
    return 0;
}
