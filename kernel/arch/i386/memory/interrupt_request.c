#include <memory/interrupt_request.h>

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

/*
 * remap the irq controllers. 
 * before we remap, IRQ0 to IRQ7 are mapped the entries 8 to
 * 15 in idt, but entries 0 to 31 is reserved for the isrs
 * if we do not remap, something must be wrong.
 * so now, we remap IRQ0-15 to entries 32 to 47 in idt.
*/
void irq_remap(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    /* just enable key board interrupt now. */
    //    outb(0x21, 0xFD);
    outb(0x21, 0xFC);
    outb(0xA1, 0xFF);

    //    irq_set_mask(2); // cascade
    //    irq_set_mask(1); // keyboard
}

/* enable an irq, bad, not work correctly*/
void irq_set_mask(unsigned char irq) {
    ushort irq_mask = (inb(0xA1)<<8) + inb(0x21);
    irq_mask &= ~(1<<irq);
    outb(0xA1, irq_mask);
    outb(0x21, irq_mask >> 8);

    /*
    int port;
    int value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) | (1 << IRQline);
    outb(port, value);*/

}

/* disenable an irq*/
void irq_clear_mask(unsigned char IRQline) {
    int port;
    int value;
 
    if(IRQline < 8) {
        port = 0x21;
    } else {
        port = 0xA1;
        IRQline -= 8;
    }
    value = inb(port) & ~(1 << IRQline);
    outb(port, value);        
}

/* install entries 32 to 47 in idt.*/
void irq_install() {
  irq_remap();

  idt_set_gate(32, (unsigned)_irq0, 0x08, 0xE, 0);
  idt_set_gate(33, (unsigned)_irq1, 0x08, 0xE, 0);
  idt_set_gate(34, (unsigned)_irq2, 0x08, 0xE, 0);
  idt_set_gate(35, (unsigned)_irq3, 0x08, 0xE, 0);
  idt_set_gate(36, (unsigned)_irq4, 0x08, 0xE, 0);
  idt_set_gate(37, (unsigned)_irq5, 0x08, 0xE, 0);
  idt_set_gate(38, (unsigned)_irq6, 0x08, 0xE, 0);
  idt_set_gate(39, (unsigned)_irq7, 0x08, 0xE, 0);
  idt_set_gate(40, (unsigned)_irq8, 0x08, 0xE, 0);
  idt_set_gate(41, (unsigned)_irq9, 0x08, 0xE, 0);
  idt_set_gate(42, (unsigned)_irq10, 0x08, 0xE, 0);
  idt_set_gate(43, (unsigned)_irq11, 0x08, 0xE, 0);
  idt_set_gate(44, (unsigned)_irq12, 0x08, 0xE, 0);
  idt_set_gate(45, (unsigned)_irq13, 0x08, 0xE, 0);
  idt_set_gate(46, (unsigned)_irq14, 0x08, 0xE, 0);
  idt_set_gate(47, (unsigned)_irq15, 0x08, 0xE, 0);

}

void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void info(struct registers *r){
    printf("gs = %d, fs = %d, es = %d, ds = %d\n", r->gs, r->fs, r->es, r->ds);
    printf("edi = %d, esi = %d, ebp = %d \n",r->edi, r->esi, r->ebp);
    printf("ebx = %d, edx = %d, ecx = %d, eax = %d \n",r->ebx, r->edx, r->ecx, r->eax);
    printf("int_no = %d, err_code = %d\n", r->int_no, r->err_code);
    printf("eip = %d, cs = %d, eflags = %d\n", r->eip, r->cs, r->eflags);
    printf("esp = %d, ss = %d \n", r->esp, r->ss);
    //    printf("interrupt_str: %s", trap_str[r->int_no]);
}


void irq_handler(struct registers *r) {
  //    printf("irq number:%d\n", r->int_no);
    //    info(r);
    void (*handler)(struct registers *r);

    //    printf("irq number:%d                      ", r->int_no);
    //    puts(" irq handling...                     ");

    handler = irq_routines[r->int_no - 32];

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outb(0x20, 0x20);
    
    if (handler)
    {
        handler(r);
        //        printf("get into handler();\n");
    }

    //    printf("irq end.\n");
}

void irq_install_handler(int irq, void (*handler)(struct registers *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}
