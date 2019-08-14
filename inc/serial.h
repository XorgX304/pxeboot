#ifndef _SERIAL_H
#define _SERIAL_H
#define COM1_PORT 0x3f8

void outb(uint16_t portid, uint8_t val);
uint8_t inb(uint16_t portid);
void serial_init(void);
void write_serial(uint8_t a);

#endif