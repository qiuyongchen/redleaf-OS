#ifndef ASM_INLINE_H
#define ASM_INLINE_H

#include <kernel/global_parameter.h>

/* load TSS into tr */
static inline void ltr(uint n){
    asm volatile("ltr %%ax"::"a"(n));
}

/* delay some time */
static inline void io_delay(){
    asm volatile (
        "jmp 1f; 1: jmp 1f; 1:"
        );
}

/* inb & outb */
static inline uchar inb(ushort port){
    uchar ret;
    asm volatile( "inb %1, %0" : "=a" (ret) : "dN" (port));
    io_delay();
    return ret;
}

static inline void outb(ushort port, uchar data){
    asm volatile( "outb %1, %0" :: "dN" (port), "a" (data));
    io_delay();
}

/* inw & outw */
static inline uchar inw(ushort port){
    ushort ret;
    asm volatile( "inw %1, %0" : "=a" (ret) : "dN" (port));
    io_delay();
    return ret;
}

static inline void outw(ushort port, ushort data){
    asm volatile( "outw %1, %0" :: "dN" (port), "a" (data));
    io_delay();
}

/* insb & outsb */
static inline void insb(uint port, void *addr, int cnt) {
	asm volatile(
            "cld;" "repne;" "insb"
			 :"=D" (addr), "=c" (cnt)
			 :"d" (port), "0" (addr), "1" (cnt)
			 :"memory", "cc");
    io_delay();
}

static inline void outsb(uint port, void *addr, int cnt) {
	asm volatile("cld;" "repne;" "outsb"
			 :"=S" (addr), "=c" (cnt)
			 :"d" (port), "0" (addr), "1" (cnt)
			 :"cc");
    io_delay();
}

/* insl & outsl */
static inline void insl(uint port, void *addr, int cnt) {
	asm volatile(
            "cld;" "repne;" "insl"
			 :"=D" (addr), "=c" (cnt)
			 :"d" (port), "0" (addr), "1" (cnt)
			 :"memory", "cc");
    io_delay();
}

static inline void outsl(uint port, void *addr, int cnt) {
	asm volatile("cld;" "repne;" "outsl"
			 :"=S" (addr), "=c" (cnt)
			 :"d" (port), "0" (addr), "1" (cnt)
			 :"cc");
    io_delay();
}

static inline void lpgd(struct pde *pgdir){
    asm volatile("mov %%eax, %%cr3":: "a"((uint)pgdir));
}

static inline void cli() {
  asm volatile("cli"); // no interrupt, or it will be more complex
}

static inline void sti() {
  asm volatile("sti"); // enable interrupt, as work done.
}

#endif