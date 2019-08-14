#ifndef _STRING_H
#define _STRING_H

#define FB_GREEN     0
#define FB_DARK_GREY 15
#define DEFAULT_RESOLVE_STACK 128
#define ERR_CODE 0x44
#define ERR_CHECKSUM 0x45

typedef unsigned int size_t;

void log_hex(uint32_t i);
char *strncpy(char *dst, const char *src, int n);
void *memset(void *s, int c, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);


#endif