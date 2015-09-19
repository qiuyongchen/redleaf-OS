/**
 * kernel initialization.
 * 
 * @author: qiuyongchen
 * @date: 08/28/2015
 * @file: kernel.c
 *
 */
 
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <boot/multiboot.h>
#include <driver/driver_keyboard.h>
#include <driver/driver_timer.h>
#include <kernel/shell.h>
#include <kernel/tty.h>
#include <memory/global_descriptor_table.h>
#include <memory/interrupt_descriptor_table.h>
#include <memory/memory.h>
#include <process/process.h>

void kernel_early(void) {
  terminal_initialize();
}

void hello() {
  for (int i = 0; i < 20; ++i)
    printf("hello, Houston, this is APO\n");
}

void hi() {
  for (int i = 0; i < 20; ++i)
    printf("hi, APO, this is Houston\n");
}

// the argument mb is used for calculating the size of the memory.
void kernel_main(unsigned int magic, multiboot_info_t *mb) {
  int *a;

  struct process *p;

  terminal_initialize();

  printf("terminal_initializeize():done...\n");

  // initialize the global_descriptor_table first
  gdt_init();
  printf("gdt_init():done...\n");

  // initialize the interrupt_descriptor_table
  idt_init();
  printf("idt_init():done...\n");

  // initialize the memory
  memory_init(mb);
  printf("memory_init():done...\n");

  // initialize the timer, 100HZ.
  timer_init();
  printf("timer_init():done...\n");  

  // initialize the keyboard, so that we can input something
  keyboard_init(); 
  printf("keyboard_init():done...\n");

  // initialize the process, prepare for process sheculing.
  process_init(); 
  printf("process_init():done...\n");  

  // init the shell.(WARNING: shell should not exit.
  create_kernel_proc(shell, 100);

  sti();

  while (1) {  
  }

}