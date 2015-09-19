#include <memory/interrupt_descriptor_table.h>

// lidt idt_desc
struct gate_desc   idt[256];
struct idt_desc    idt_desc;

/* ------------------------------------------------------------------- */

void idt_set_gate(uint nr, unsigned long base, ushort sel, uchar type, uchar dpl) {
    idt[nr].base_lo    = (base & 0xFFFF);
    idt[nr].base_hi    = (base >> 16) & 0xFFFF;
    idt[nr].sel        = sel;
    idt[nr].dpl        = dpl;
    idt[nr].type       = type;
    idt[nr].always0    = 0;
    idt[nr].p          = 1;
    idt[nr].sys        = 0;
}

/* load the idt descriptor into cpu */
void lidt(struct idt_desc idtd){
    asm volatile(
        "lidt %0"
        :: "m"(idtd));
}

/* clear out the entire IDT to zero before it was installed irq or isrs.*/
void idt_clear() {
    memset(&idt, 0, sizeof(struct gate_desc) * 256);
}

void idt_init(){
    idt_desc.limit = (sizeof(struct gate_desc) * 256) - 1;
    idt_desc.base = (uint)&idt;

    idt_clear();

    /* install the first 32 entries into the idt */
    isrs_install();

    /* install the entries 32 to 47 in the idt */
    //    irq_install();

    irq_install();

    //    hwint_init();
    lidt(idt_desc);
}
