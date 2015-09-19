#ifndef PAGING_H
#define PAGING_H

#include <kernel/asm_inline.h>
#include <memory/interrupt_service_routines.h>

/* assuem that the kernel take 64 megabytes*/
#define KERNEL_END 0X4000000

/* page directory entry*/
struct pde {
    uint      pd_flag:9;
    uint      pd_avl:3;
    uint      pd_off:20;
};

/* page table entry*/
struct pte {
    uint      pt_flag:9;
    uint      pt_avl:3;
    uint      pt_off:20;
};

/* the virtual memory block, each page's size is 4KB.*/
struct page {
  char page_flag;
  short page_count;
  int page_num;
  struct page *next_page;
};

/* the map of the pages(each page has size 4KB)*/
struct page map[(4 * 1024 * 1024) / (4)];

/* a list that link all the free page, only the page in this list can be allocted.*/
struct page free_page_list;

/* the first page directory */
struct pde page_dir[1024] __attribute__((aligned(4096)));

void paging_init();

void page_dir_init();

void set_page_fault_handler();

void mmu_enable();

struct page* allocate_page();

int free_page(struct page* p);

void allocate_free_page_test();

void no_page(int addr);

void wp_page(int addr);

void page_fault_handler(struct registers *r);

int page_table_copy(struct pde *des, struct pde *src);

#endif
