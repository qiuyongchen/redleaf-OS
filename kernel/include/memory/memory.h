#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#include <boot/multiboot.h>
#include <kernel/global_parameter.h>
#include <kernel/asm_inline.h>
#include <memory/paging.h>
#include <memory/liballoc.h>
//#include <process/multitasking.h>
#include <process/process.h>

#define DEBUG

/* the size in KB of upper memory above the first 1 megabyte.*/
int MEM_UPPER;
/* the size in KB of lower memory which starts at address 0.*/
int MEM_LOWER;
/* the size in KB of the physical memory.*/
int MEM_SIZE;

void memory_init(multiboot_info_t *mb);

void get_memory_size(multiboot_info_t *mb);

void allocate_test();

//int ms_copy(struct memory_space *des);

#endif
