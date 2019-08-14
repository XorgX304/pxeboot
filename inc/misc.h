#ifndef _MISC_H
#define _MISC_H

#include "pxe.h"
#include "string.h"


uint64_t read_msr_new(unsigned int msr);

uint8_t 
checksum(void *penv, size_t len);

void 
pxe_proc();

static uint16_t 
__bswap_16 (uint16_t __bsx);

uint16_t 
htons (uint16_t x);

void 
go_to_oppalara();

void 
tftp_read_file(char *dst, struct PXE *pxe, uint16_t packetSize);

void 
call_rm(struct pxe_ret *regs);


#define __bswap_constant_16(x)                                        \
  ((uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))


#endif