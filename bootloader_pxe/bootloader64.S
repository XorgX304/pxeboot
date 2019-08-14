#include "../def.h"

/*
* This project is contains few ZeldaOS (kudos to Jie Zheng), GCC procedures
*/
.code16

.equ MY_STACK, 0x7C00
.org 0
bootloader_init:
    ljmp $0x7c0, $0x0d

.org 0x0d
bootloader_main:
    cli


    xor %ax, %ax
    mov %ax, %ss
    mov $MY_STACK, %ax
    mov %ax, %sp
    mov $0x07c0, %ax
    mov %ax, %ds
    
    call load_gdt
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0
    movw $0x10, %dx
    movw %dx, %fs
    movw %dx, %gs
    andl $0xfffffffe, %eax
    movl %eax, %cr0

    // Detect physical memory list
    call detect_physical_memory

    // Load first stage to memory
    call load

    // Jump to first stage
    jmp call_it




load:
    pushal
    cld
    movl $PXE_FIRST, %edi
    movl $entry, %esi
    xor %ecx, %ecx
    movl $end_byte, %ecx

    cause_love_you:
    movb %ds:(%esi), %al
    movb %al, %fs:(%edi)
    xchgw %bx, %bx
    inc %esi
    inc %edi
    dec %ecx
    jne cause_love_you
    popal
    ret


call_it:
    mov $0x7c00, %esp
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0
    movw $0x10, %dx
    movw %dx, %ds
    movw %dx, %ss
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    ljmpl $0x08, $PXE_FIRST




/*
 * Detect physical memory with BIOS functions
 * https://wiki.osdev.org/Detecting_Memory_(x86)#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820
 */
detect_physical_memory:
    pushal
    movw $0x7c0, %ax
    movw %ax, %es
    movw $MEMORY_LIST_BASE, %di
    movw $512, %cx
1:
    movl $0x0, %es:(%di)
    movl $0x0, %es:4(%di)
    addw $8, %di
    loop 1b
    movw $MEMORY_LIST_BASE, %di
    xorl %ebx, %ebx
2:
    movl $0x534d4150, %edx
    movl $24, %ecx
    movl $0xe820, %eax
    int $0x15
    addw $24, %di
    cmpl $0x0, %ebx
    jnz 2b
 
    popal
    ret



pxe_struct:
    pushal
    xor %eax,%eax
    movw $0x5650, %ax
    int $0x13
    popal
    ret


load_gdt:
    pushal
    movw $gdt_info_offset, %di
    movl $GDT_BASE, (%di)
    addl $0x7c00, (%di)
    movl $GDT_INFO, %eax
    xchg %bx, %bx
    lgdt (%eax)
    popal
    ret


.align 8
GDT_INFO:
    gdt_info_size: .word 0x18
    gdt_info_offset: .long 0x0

.align 8
GDT_BASE:
unsed_descriptor:
    .long 0x0
    .long 0x0
kernel_code_descriptor:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x9a
    .byte 0xcf
    .byte 0x0
kernel_data_descriptor:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x92
    .byte 0xcf
    .byte 0x0
.align 4
 
// Let the last two bytes be 0x55AA
.org 0x1fe
    .byte 0x55
    .byte 0xaa


.org 0x400

.code32
entry:
    .incbin "pxe_boot.bin"

end_byte:
    .long . - entry
