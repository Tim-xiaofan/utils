#ifndef _UTILS_H
#define _UTILS_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int hexstr2bin(const uint8_t *buf, int length, uint8_t * bin, int size);
extern int bin2hexstr(const uint8_t *bin, int length, uint8_t * buf, int size);

#ifdef __cplusplus
}
#endif

#endif
