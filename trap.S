#include "def.h"
.code32

.global call_rm 
call_rm:
	pushal
	mov %esp, %ebp
    movl $RM_GDT_INFO, %eax
    lgdt (%eax)
	movw $0x10, %ax
	movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    leal (callfn - PXE_FIRST), %edx
    jmp $0x08, $halting - PXE_FIRST


.code16
halting:
	movl %cr0, %eax
    andl $~0x1, %eax
    movl %eax, %cr0

	xorw %ax, %ax
	movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    xor %esi,%esi
	movl $0x10000, %esi
	shr $0x4,%esi

	pushf
	pushw %si
	pushw %dx
	lretw
callfn:
	xor %eax,%eax
	movw $0x5650, %ax
	int $0x1A

    #we pushed 8 GPR to stack with pushal(each of registers size is 4 byte = 4*8)
	movl 4+24(%ebp), %edi
	movw %es, (%edi)
	movl %ebx, 2(%edi)
	
	mov %esi,%ds

	movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0

	movl $(PM_GDT_INFO - PXE_FIRST), %edx
    lgdt (%edx)
	
	xor %edx,%edx
    movw $0x10, %dx
    movw %dx, %ds
    movw %dx, %ss
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    xor %ebx,%ebx
	mov $future, %ebx

    //back to the future 
	ljmpl $0x08, $future

.code32
future:
	mov %ebp, %esp
	popal
	ret




.code32

.global pxe_proc 
pxe_proc:
    pushal
    mov %esp, %ebp
    movl $RM_GDT_INFO, %eax
    lgdt (%eax)
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    leal (pxe_second - PXE_FIRST), %edx
    jmp $0x08, $pxe_first - PXE_FIRST



.code16
pxe_first:
    movl %cr0, %eax
    andl $~0x1, %eax
    movl %eax, %cr0

    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %ss
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    xor %esi,%esi
    movl $0x10000, %esi
    shr $0x4,%esi

    pushf
    pushw %si
    pushw %dx
    lretw


pxe_second:
    cli
    mov 0x24(%ebp), %bx
    mov 4+0x24(%ebp), %di
    push %ds
    push %di
    push %bx
    lcall *0x8+0x24(%ebp)
    add $6, %esp


    movl $0x10000, %eax
    shr $0x4,%eax                                                                               
    mov %eax,%ds


    xor %eax,%eax
    
    movl %cr0, %eax                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
    orl $0x1, %eax
    movl %eax, %cr0

    movl $(PM_GDT_INFO - PXE_FIRST), %edx
    
    lgdt (%edx)
    
    xor %edx,%edx
    movw $0x10, %dx 
    movw %dx, %ds
    movw %dx, %ss
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs
    xor %ebx,%ebx
    mov $pxe_third, %ebx
    //back to the future 
    ljmpl $0x08, $pxe_third

.code32
pxe_third:
    mov %ebp, %esp
    popal
    ret


.code32
.global go_to_oppalara
go_to_oppalara:
    ljmpl $0x08, $KERNEL_BSP_BASE


.align 8
RM_GDT_BASE:
rm_unsed_descriptor:
    .long 0x0
    .long 0x0
rm_kernel_code_descriptor:
    .word 0xffff
    .word 0x0000
    .byte 0x01
    .byte 0x9a
    .byte 0x00
    .byte 0x00
rm_kernel_data_descriptor:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x92
    .byte 0x0
    .byte 0x0
.align 8

RM_GDT_INFO:
    rm_gdt_info_size: .word 0x18
    rm_gdt_info_offset: .long RM_GDT_BASE





.align 8
PM_GDT_BASE:
pm_unsed_descriptor:
    .long 0x0
    .long 0x0
pm_kernel_code_descriptor:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x9a
    .byte 0xcf
    .byte 0x0
pm_kernel_data_descriptor:
    .word 0xffff
    .word 0x0
    .byte 0x0
    .byte 0x92
    .byte 0xcf
    .byte 0x0

.align 8
PM_GDT_INFO:
    pm_gdt_info_size: .word 0x18
    pm_gdt_info_offset: .long PM_GDT_BASE
