#include <stdint.h>
#include "inc/misc.h"
#include "inc/string.h"


uint8_t checksum(void *penv, size_t len){
    uint8_t *bytes = penv;
    uint8_t sum = 0;

    while ( len-- ) {
        sum += *bytes++;
    }
    return sum;
}

static inline uint16_t __bswap_16 (uint16_t __bsx)
{
  return __bswap_constant_16 (__bsx);
}


uint16_t htons (uint16_t x)
{
  return __bswap_16 (x);
}
