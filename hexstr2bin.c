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

static int get_char(int i)
{
    if(i >= 0 && i <= 9)
      return '0' + i;
    else if(i >= 10 && i <= 15)
      return 'a' + i - 10;
    else 
      return '#';
}

int hexstr2bin(const uint8_t *buf, int length, uint8_t * bin, int size)
{
    int i, copied;
    uint8_t h, l;

    if(length % 2 != 0) return 0;

    for(i = 0, copied = 0; 
                i < length && copied < size; 
                i += 2, ++copied)
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

int bin2hexstr(const uint8_t *bin, int length, uint8_t * buf, int size)
{
    int i, copied;

    for(i = 0, copied = 0; 
                i < length && copied < size -1; 
                ++i, copied += 2)
    {
        buf[copied] = get_char(bin[i] / 16);
        //printf("%c\n", buf[copied]);
        buf[copied + 1] = get_char(bin[i] % 16);
        //printf("%c\n", buf[copied + 1]);
    }
    return copied;
}
