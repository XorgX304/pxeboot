#include <stdint.h>
#include "inc/string.h"
#include "inc/serial.h"
unsigned int strlen(const char *s)
{
    size_t len = 0;

    for (; *s; ++s) {
        ++len;
    }

    return len;
}

void log_hex(uint32_t i)
{
    char *digits = "0123456789ABCDEF";
    uint32_t n, digit, min_digits = 8;
    int x=0;

    /* find the largest nibble to output */
    if (i >= 0x10000000) {
        n = 28;
    } else {
        n = 0;
        while ((((uint32_t)0x01) << (n+4)) <= i) {
            n += 4;
        }
    }


    /* pad with zeroes */
    if (min_digits > 0) {
        min_digits -= 1;
    }
    min_digits <<= 2;
    while (min_digits > n) {
        min_digits -= 4;
    }
    /* print the number */
    while (1) {
        digit = (i >> n) & 0x0000000F;
        //write_serial(digits[digit]);
        fb_write_cell((x++)*2, digits[digit], FB_GREEN, FB_DARK_GREY);
        if (n == 0) {
            break;
        }
        n -= 4;
    }
    //write_serial('\n');
}


int strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    for (i = 0; i < n; ++i) {
        if (*s1 == *s2) {
            continue;
        }

        if (*s1 == '\0' && *s2 != '\0') {
            return -1;
        }

        if (*s2 == '\0' && *s1 != '\0') {
            return 1;
        }

        if (*s1 < *s2) {
            return -1;
        } else {
            return 1;
        }
    }

    return 0;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *base = (uint8_t *) s;
    uint8_t *p;
    uint8_t cb = (uint8_t) (c & 0xFF);

    for (p = base; p < base + n; ++p) {
        *p = cb;
    }

    return s;
}

char *strncpy(char *dst, const char *src, int n)
{
    char *tmp = dst;

    while (n) {
        *tmp = *src;
        if (*tmp)
            src++;
        tmp++;
        n--;
    }

    return dst;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *to = (uint8_t *) dest;
    uint8_t *from = (uint8_t *) src;
    size_t i;

    for(i = 0; i < n; ++i) {
        *to++ = *from++;
    }

    return dest;
}

