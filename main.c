#include <stdint.h>
#include "def.h"
#include "inc/string.h"
#include "inc/serial.h"
#include "inc/pxe.h"
#include "inc/misc.h"


void entry(void){
    struct pxe_ret regs;
    call_rm(&regs);
    struct PXE_ENV *pxe_environment = ((struct PXE_ENV*)((regs.es << 4) + regs.ebx));
    if(strncmp(pxe_environment->Signature,"PXENV+",5) != 0){
        log_hex(ERR_CODE);
        //boot aborted
        __asm__("hlt");
        return;
    }
    if(checksum(pxe_environment, pxe_environment->Length) != 0){
        log_hex(ERR_CHECKSUM);
        __asm__("hlt");
        return;
    }
    struct PXE *pxe = (struct PXE*)((pxe_environment->PXEPtr.seg << 4) + pxe_environment->PXEPtr.offset);
    if(strncmp(pxe->Signature, "!PXE", 4) != 0){
        log_hex(ERR_CODE);
        __asm__("hlt");
        return;
    }
    if(checksum(pxe, pxe->Length) != 0){
        log_hex(ERR_CHECKSUM);
        __asm__("hlt");
        return;
    }
    struct cache_info cache_str;
    memset(&cache_str,0, sizeof(cache_str));
    cache_str.PacketType = 2;
    pxe_proc(0x0071, &cache_str, pxe->RMEntry);
    uint8_t *serverip = (uint8_t*)(((cache_str.Buffer.seg << 4) + cache_str.Buffer.offset) + 0x14);


    struct tftp_getsize getsize;
    memset(&getsize, 0, sizeof(getsize));
    getsize.ServerIPAddress[0] = serverip[0];
    getsize.ServerIPAddress[1] = serverip[1];
    getsize.ServerIPAddress[2] = serverip[2];
    getsize.ServerIPAddress[3] = serverip[3];
    char* bname = "vmxer.bin";

    char* b = getsize.FileName;
    while (*bname)
        *b++ = *bname++;
	pxe_proc(0x0025, &getsize, pxe->RMEntry);


    struct tftp_open open_stream;
    char* fname = "vmxer.bin";
    memset(&open_stream, 0, sizeof(open_stream));
    open_stream.ServerIPAddress[0] = serverip[0];
    open_stream.ServerIPAddress[1] = serverip[1];
    open_stream.ServerIPAddress[2] = serverip[2];
    open_stream.ServerIPAddress[3] = serverip[3];
    open_stream.TFTPPort = htons(69);
    open_stream.PacketSize = 512;
    
    char* p = open_stream.FileName;
    while (*fname)
        *p++ = *fname++;
    
    pxe_proc(0x0020, &open_stream, pxe->RMEntry);

    char *kernel = (char*)KERNEL_BSP_BASE;
    tftp_read_file(kernel, pxe, open_stream.PacketSize);

    struct tftp_close close = {0};
	pxe_proc(0x0021, &close, pxe->RMEntry);
	
	go_to_oppalara();

    __asm__("hlt");

}


void tftp_read_file(char *dst, struct PXE *pxe, uint16_t packetSize){
	struct tftp_read tftpread;
	uint8_t buf[512];
	while(1){
		memset(&tftpread, 0, sizeof(tftpread));

		tftpread.BuffPTR.offset = (uint16_t)(uint32_t)buf;
		tftpread.BuffPTR.seg = 0;
	    pxe_proc(0x0022, &tftpread, pxe->RMEntry);
		memcpy((char*)dst, buf, 512);
	 	dst+=512;
		
	    if(packetSize =! tftpread.BuffSize){
	    	break;
		}
	}

}
