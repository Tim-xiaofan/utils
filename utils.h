#ifndef _HEXSTR2BIN_H
#define _HEXSTR2BIN_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    
extern int hexstr2bin(uint8_t *buf, int length, uint8_t * bin, int size);

#ifdef __cplusplus
}
#endif

#endif
