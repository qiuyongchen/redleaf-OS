#ifndef INTERRUPT_DESCRIPTOR_TABLE_H
#define INTERRUPT_DESCRIPTOR_TABLE_H

#include <string.h>
#include <kernel/asm_inline.h>
#include <kernel/global_parameter.h>
#include <kernel/x86_cpu.h>
#include <memory/global_descriptor_table.h>
#include <memory/interrupt_service_routines.h>
#include <memory/interrupt_request.h>

struct gate_desc {
    uint        base_lo :16;        // low address
    uint        sel     :16;        // selector
    uint        always0 :8;         // reserved
    uint        type    :4;         // type(STS_{TG,IG32,TG32})
    uint        sys     :1;         // must be 0 (system)
    uint        dpl     :2;         // descriptor(meaning new) privilege level
    uint        p       :1;         // Present
    uint        base_hi :16;        // high address
    } __attribute__((packed));

struct idt_desc {
    ushort      limit;
    uint        base;
} __attribute__((packed));

struct registers {
    int        gs, fs, es, ds;
    int        edi, esi, ebp, _esp, ebx, edx, ecx, eax;
    int        int_no, err_code;
    int        eip, cs, eflags, esp, ss;
} __attribute__((packed));

/* constants on PIC */

#define IRQ0 32
#define IRQ_SLAVE 2

/* function prototypes */

void idt_set_gate(uint nr, unsigned long base, ushort sel, uchar type, uchar dpl);

void lidt(struct idt_desc idtd);

void idt_clear();

void idt_init();

#endif
