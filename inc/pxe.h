#ifndef _PXE_H
#define _PXE_H

struct pxe_ret{
    uint16_t es;
    uint16_t ebx; 
};

typedef struct{
    uint16_t offset;
    uint16_t seg;
} ptr_pxe;

struct __attribute__((__packed__)) PXE_ENV
{
    // The Signature of the PXENV+ structure - contains "PXENV+".
    uint8_t  Signature[6];

    // The MSB contains the Major version number, while the LSB contains the minor version number.
    uint16_t Version;

    // The Length and the checksum of the structure, used for calculating the checksum.
    uint8_t  Length;
    uint8_t  Checksum;

    // A Far Pointer to the real mode PXE API entry point. 
    uint32_t RMEntry;

    // 32-bit offset to the protected mode API entry point. Instead of this, the !PXE structure is recommended.
    uint32_t PMOffset;

    // The rest of the fields don't matter much, and the Specifications can be referred.
    uint16_t PMSelector;
    uint16_t StackSeg;
    uint16_t StackSize;
    uint16_t BCCodeSeg;
    uint16_t BCCodeSize;
    uint16_t BCDataSeg;
    uint16_t BCDataSize;

    uint16_t UNDIDataSeg;
    uint16_t UNDIDataSize;
    uint16_t UNDICodeSeg;
    uint16_t UNDICodeSize;

    // This is a far pointer to the "!PXE" structure, only present when the structure is present.
    ptr_pxe PXEPtr;
};  



struct __attribute__((__packed__)) PXE{

    // The Signature of the !PXE structure - contains "!PXE".
    uint8_t  Signature[4];

    // The Length and the checksum of the structure, used for calculating the checksum.
    uint8_t  Length;
    uint8_t  Checksum;

    // Contains zero - both of them.
    uint8_t Revision;
    uint8_t Reserved;

    // Far pointer to UNDI ROM ID structure and BC ROM ID structure.
    uint32_t UNDIROMID;
    uint32_t BCROMID;

    // A Far Pointer to the real mode PXE API entry point. 
    uint32_t RMEntry;

    // A Far Pointer to the protected mode API entry point. 
    uint32_t PMEntry;

};

struct __attribute__((__packed__)) cache_info
{
    uint16_t    Status;
    uint16_t    PacketType;
    uint16_t    BufferSize;
    ptr_pxe     Buffer;
    uint16_t    BufferLimit;
};

struct __attribute__((__packed__)) tftp_open
{

    uint16_t Status;
    uint8_t ServerIPAddress[4];
    uint8_t GatewayIPAddress[4];
    char FileName[128];
    uint16_t TFTPPort;
    uint16_t PacketSize;
};

struct __attribute__((__packed__)) tftp_read
{
    uint16_t    Status;
    uint16_t    PacketNum;
    uint16_t    BuffSize;
    ptr_pxe BuffPTR;
};


struct __attribute__((__packed__)) tftp_getsize
{

	uint16_t Status;
    uint8_t ServerIPAddress[4];
    uint8_t GatewayIPAddress[4];
    char FileName[128];
	uint32_t FileSize;
};



struct __attribute__((__packed__)) tftp_close
{
    uint16_t status;
};

#endif