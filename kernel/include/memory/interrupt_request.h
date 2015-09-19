#ifndef INTERRUPT_REQUEST_H
#define INTERRUPT_REQUEST_H

#include <stdio.h>

#include <memory/interrupt_descriptor_table.h>
#include <memory/interrupt_service_routines.h>
#include <kernel/asm_inline.h>

void irq_remap(void);

void irq_set_mask(unsigned char IRQline);

void irq_clear_mask(unsigned char IRQline);

void irq_install();

void irq_handler(struct registers *r);

void irq_install_handler(int irq, void (*handler)(struct registers *r));

void irq_uninstall_handler(int irq);

#endif
