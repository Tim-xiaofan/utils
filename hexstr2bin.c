#include "utils.h"
#include <stdio.h>

static int get_int(char c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    else if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return 0;
}

int hexstr2bin(uint8_t *buf, int length, uint8_t * bin, int size)
{
    int i = 0, copied;
    uint8_t h, l;

    if(length % 2 != 0) return 0;

    for(i = 0, copied = 0; 
                i < length; 
                i = i + 2, ++copied)
    {
        bin[copied] = 0;
        h = get_int(buf[i]);
        l = get_int(buf[i + 1]);
        //printf("i=%c, i+1=%c\n", buf[i], buf[i+1]);
        //printf("h = %02x,l = %02x\n", h, l);
        //printf("%x\n", (h << 4));
        //printf("%x\n", l);
        bin[copied] |= (h << 4) |l;
        //printf("sum : %02x\n", (h << 4) | l);
        //printf("bin[%d]=%02x\n\n", copied, bin[copied]);
    }

    return copied;
}
