#ifndef INTERRUPT_SERVICE_ROUTINES_H
#define INTERRUPT_SERVICE_ROUTINES_H

#include <stdio.h>

#include <memory/interrupt_descriptor_table.h>

void isrs_install();

void isr_handler(struct registers *r);

#endif
