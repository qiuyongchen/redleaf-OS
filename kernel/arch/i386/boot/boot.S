# Declare constants used for creating a multiboot header.
.set MULTIBOOT_PAGE_ALIGN,    1<<0             # align loaded modules on page boundaries
.set MULTIBOOT_MEMORY_INFO,  1<<1             # provide memory map
.set MULTIBOOT_HEADER_FLAGS,    MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO  # this is the Multiboot 'flag' field
.set MULTIBOOT_HEADER_MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set MULTIBOOT_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) # checksum of above, to prove we are multiboot

	
# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_CHECKSUM

	
# Reserve a stack for the initial thread.
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 4096 # 4 KiB
stack_top:


# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp

	# Initialize the core kernel before running the global constructors.
	#call kernel_early

	# Call the global constructors.
	call _init

	push %ebx
	push %eax
	
	# Transfer control to the main kernel.
	call kernel_main

.global _hi
.type _hi, @function
_hi:
	mov %ax, 0x3
	
.Lhang:
	jmp .Lhang


.size _start, . - _start

.extern gdt_desc

.global gdt_reflesh
.type gdt_reflesh, @function
gdt_reflesh:
    lgdt (gdt_desc)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    jmp $0x08, $reflesh
reflesh:
    ret
	
.global _isr0
.type _isr0, @function
_isr0:	
        cli
	push $0
	push $0
	jmp isr_common_stub

.global _isr1
.type _isr1, @function
_isr1:	
        cli
	push $0
	push $1
	jmp isr_common_stub
    
.global _isr2
.type _isr2, @function
_isr2:	
        cli
	push $0
	push $2
	jmp isr_common_stub
    
.global _isr3
.type _isr3, @function
_isr3:	
        cli
	push $0
	push $3
	jmp isr_common_stub
    
.global _isr4
.type _isr4, @function
_isr4:	
        cli
	push $0
	push $4
	jmp isr_common_stub
    
.global _isr5
.type _isr5, @function
_isr5:	
        cli
	push $0
	push $5
	jmp isr_common_stub
    
.global _isr6
.type _isr6, @function
_isr6:	
        cli
	push $0
	push $6
	jmp isr_common_stub
    
.global _isr7
.type _isr7, @function
_isr7:	
        cli
	push $0
	push $7
	jmp isr_common_stub
    
.global _isr8
.type _isr8, @function
_isr8:	
        cli
	push $8
	jmp isr_common_stub
    
.global _isr9
.type _isr9, @function
_isr9:	
        cli
	push $0
	push $9
	jmp isr_common_stub
    
.global _isr10
.type _isr10, @function
_isr10:	
        cli
	#push $0
	push $10
	jmp isr_common_stub
    
.global _isr11
.type _isr11, @function
_isr11:	
        cli
	push $0
	push $11
	jmp isr_common_stub
    
.global _isr12
.type _isr12, @function
_isr12:	
        cli
	push $0
	push $12
	jmp isr_common_stub
    
.global _isr13
.type _isr13, @function
_isr13:	
        cli
	push $0
	push $13
	jmp isr_common_stub
    
.global _isr14
.type _isr14, @function
_isr14:	
        cli
	#push $0
	push $14
	jmp isr_common_stub
    
.global _isr15
.type _isr15, @function
_isr15:	
        cli
	push $0
	push $15
	jmp isr_common_stub
    
.global _isr16
.type _isr16, @function
_isr16:	
        cli
	push $0
	push $16
	jmp isr_common_stub
    
.global _isr17
.type _isr17, @function
_isr17:	
        cli
	push $0
	push $17
	jmp isr_common_stub
    
.global _isr18
.type _isr18, @function
_isr18:	
        cli
	push $0
	push $18
	jmp isr_common_stub
    
.global _isr19
.type _isr19, @function
_isr19:	
        cli
	push $0
	push $19
	jmp isr_common_stub
    
.global _isr20
.type _isr20, @function
_isr20:	
        cli
	push $0
	push $20
	jmp isr_common_stub
    
.global _isr21
.type _isr21, @function
_isr21:	
        cli
	push $0
	push $21
	jmp isr_common_stub
    
.global _isr22
.type _isr22, @function
_isr22:	
        cli
	push $0
	push $22
	jmp isr_common_stub
    
.global _isr23
.type _isr23, @function
_isr23:	
        cli
	push $0
	push $23
	jmp isr_common_stub
    
.global _isr24
.type _isr24, @function
_isr24:	
        cli
	push $0
	push $24
	jmp isr_common_stub
    
.global _isr25
.type _isr25, @function
_isr25:	
        cli
	push $0
	push $25
	jmp isr_common_stub
    
.global _isr26
.type _isr26, @function
_isr26:	
        cli
	push $0
	push $26
	jmp isr_common_stub
    
.global _isr27
.type _isr27, @function
_isr27:	
        cli
	push $0
	push $27
	jmp isr_common_stub
    
.global _isr28
.type _isr28, @function
_isr28:	
        cli
	push $0
	push $28
	jmp isr_common_stub
    
.global _isr29
.type _isr29, @function
_isr29:	
        cli
	push $0
	push $29
	jmp isr_common_stub
    
.global _isr30
.type _isr30, @function
_isr30:	
        cli
	push $0
	push $30
	jmp isr_common_stub
    
.global _isr31
.type _isr31, @function
_isr31:	
        cli
	push $0
	push $31
	jmp isr_common_stub
	
.extern isr_handler

.global isr_common_stub
.type isr_common_stub, @function
isr_common_stub:	
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov $isr_handler, %eax
    call *%eax
    pop %eax
#    call *%eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

# irq functions, when there is an irq*, cpu will look for
# a function from here
.global _irq0
.type _irq0, @function
_irq0:	
#    cli
    push $0
    push $32
    jmp irq_common_stub

.global _irq1
.type _irq1, @function
_irq1:	
#    cli
    push $0
    push $33
    jmp irq_common_stub

.global _irq2
.type _irq2, @function
_irq2:	
    cli
    push $0
    push $34
    jmp irq_common_stub

.global _irq3
.type _irq3, @function
_irq3:	
    cli
    push $0
    push $35
    jmp irq_common_stub

.global _irq4
.type _irq4, @function
_irq4:	
    cli
    push $0
    push $36
    jmp irq_common_stub

.global _irq5
.type _irq5, @function
_irq5:	
    cli
    push $0
    push $37
    jmp irq_common_stub

.global _irq6
.type _irq6, @function
_irq6:	
    cli
    push $0
    push $38
    jmp irq_common_stub

.global _irq7
.type _irq7, @function
_irq7:	
    cli
    push $0
    push $39
    jmp irq_common_stub

.global _irq8
.type _irq8, @function
_irq8:	
    cli
    push $0
    push $40
    jmp irq_common_stub

.global _irq9
.type _irq9, @function
_irq9:	
    cli
    push $0
    push $41
    jmp irq_common_stub

.global _irq10
.type _irq10, @function
_irq10:	
    cli
    push $0
    push $42
    jmp irq_common_stub

.global _irq11
.type _irq11, @function
_irq11:	
    cli
    push $0
    push $43
    jmp irq_common_stub

.global _irq12
.type _irq12, @function
_irq12:	
    cli
    push $0
    push $44
    jmp irq_common_stub

.global _irq13
.type _irq13, @function
_irq13:	
    cli
    push $0
    push $45
    jmp irq_common_stub

.global _irq14
.type _irq14, @function
_irq14:	
    cli
    push $0
    push $46
    jmp irq_common_stub

.global _irq15
.type _irq15, @function
_irq15:	
    cli
    push $0
    push $47
    jmp irq_common_stub


.extern irq_handler
	
.type irq_common_stub, @function
irq_common_stub:
    sti
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov $irq_handler, %eax
    call *%eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

#        
#convert(struct context *src, struct context *des)
#
.global convert
.type convert, @function
convert:                 
    # save src's registers
    movl 4(%esp), %eax
    popl 0(%eax)                # save eip

    movl %esp, 4(%eax)
    movl %ebx, 8(%eax)        
    movl %ecx, 12(%eax)
    movl %edx, 16(%eax)
    movl %esi, 20(%eax)
    movl %edi, 24(%eax)
    movl %ebp, 28(%eax)

    # restore des's registers
    movl 4(%esp), %eax

    movl 28(%eax), %ebp
    movl 24(%eax), %edi        
    movl 20(%eax), %esi
    movl 16(%eax), %edx
    movl 12(%eax), %ecx        
    movl 8(%eax), %ebx
    movl 4(%eax), %esp

    pushl 0(%eax)

    ret

        
#.extern proc_exit
        
.global k_proc_entry
.type k_proc_entry, @function
k_proc_entry: 
    call *%ebx
#    pushl %eax        
    mov $proc_exit, %ecx
    call *%ecx
