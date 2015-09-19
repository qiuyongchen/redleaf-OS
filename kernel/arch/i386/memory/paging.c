#include <memory/memory.h>

void paging_init() {
  int page_n;
  struct page *free_page_list_h;
  struct page *temp_page;

  int physical_memory;
  int begin_addr, end_addr;

  /* figure out how many pages can we have. */
  //  for (int i = 0; i < MEM_UPPER / 4; ++i) {
  //    map[i].page_flag = 8; // (00001000)this page is physical page!
  //  }

  /* reserve kernel pages, or they will be allocted and make some wrong.*/
  for (page_n = 0; page_n < KERNEL_END / 4096; ++page_n) {
    map[page_n].page_flag = 0x2; // (00001010)this page is reserve
    map[page_n].page_count = 100;
    map[page_n].page_num = page_n;
  }

  /* get free & exit pages together.*/
  free_page_list_h = &free_page_list;
  free_page_list_h->next_page = NULL;

  physical_memory = MEM_UPPER / 4096;
  physical_memory *= 1024;

  for (; page_n < physical_memory; ++page_n) {
    temp_page = &map[page_n];

    temp_page->page_flag = 0;
    temp_page->page_count = 0;
    temp_page->page_num = page_n;

    temp_page->next_page = free_page_list_h->next_page;
    free_page_list_h->next_page = temp_page;

  }

  begin_addr = &map[0];
  end_addr = &map[1024 * 1024];
  printf("paging_init():done...physical memory map:begin address:%d (bytes), end address:%d (bytes)\n", begin_addr / 1024, end_addr / 1024);
}

void page_dir_init(struct pde * page_dir) {
  int begin_addr, end_addr;

  /* map virtul (size as the physical) memory to kernel space.*/
  for (int i = 0; i < MEM_UPPER / 4096; ++i) {
    page_dir[i].pd_off = i << 10;
    page_dir[i].pd_flag = 0x83; // prsent;writable;big size;
  }

  /* map the rest virtual memory to userspacse.*/
  for (int i = MEM_UPPER / 4096; i < 1024; ++i) {
    page_dir[i].pd_off = 0;
    page_dir[i].pd_flag = 0X4; // user;
  }

  begin_addr = page_dir;
  printf("page_dir_init():done...virtual memory map:begin address:%d(bytes), end address:%d (bytes)\n", begin_addr / 1024, (begin_addr + sizeof(struct pde) * 1024 * 1024) / 1024);

}

void set_page_fault_handler() {
  idt_set_gate(14, (unsigned)page_fault_handler, 0x08, 0xF, 0);
}

void mmu_enable(){
    uint cr0, cr4;
    asm volatile("mov %%cr4, %0": "=r"(cr4));
    cr4 |= 0x10;// big pages
    asm volatile("mov %0, %%cr4":: "r"(cr4));
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;// enable paging
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

/* allocate a free page, return a pointer to that page.*/
struct page* allocate_page() {
  struct page* p;

  if (free_page_list.next_page == NULL) {
    printf("ERROR:NO FREE PHYSICAL PAGE\n");
    return NULL;
  }

  //  cli();

  /* take out an page from free page list.*/
  p = free_page_list.next_page;
  free_page_list.next_page = p->next_page;
  p->page_count = 1;

  //  sti();

  return p;
}

/* if nobody referrence to a page, then free that page, return its page_num */
int free_page(struct page* p) {

  if (p->page_count == 0) {
    printf("ERROR: THE PAGE IS FREE BEFORE, CAN NOT FREE AGAIN\n");
  }

  //cli();

  --p->page_count;
  if (p->page_count == 0) {
    p->next_page = free_page_list.next_page;
    free_page_list.next_page = p;
  }

  //sti();

  return p->page_num;
}

void allocate_free_page_test() {
  struct page * p;
  int num = 0;
  struct page *temp = &free_page_list;

  while(temp->next_page != NULL) {
    ++num;
    temp = temp->next_page;
  }
  printf("there are %d free pages.\n", num);

  printf("allocate_free_page_test():try to allocate a page\n");

  p = allocate_page();
  if (p != NULL) {
    printf("allocate_free_page_test():success to allocate a page\n");
    printf("allocate_free_page_test():page_num:%d page_count:%d\n", p->page_num, p->page_count);
    printf("allocate_free_page_test():try to free that page\n");
    printf("allocate_free_page_test():page_num:%d\n", free_page(p));

    printf("allocate_free_page_test():try to allocate a page again\n");
    p = allocate_page();
    printf("allocate_free_page_test():success to allocate a page\n");
    printf("allocate_free_page_test():page_num:%d page_count:%d\n", p->page_num, p->page_count);
    printf("allocate_free_page_test():try to free that page\n");
    printf("allocate_free_page_test():page_num:%d\n", free_page(p));

  } else {
    printf("allocate_free_page_test():fail to allocate a page\n");
  }

  temp = &free_page_list;
  num = 0;
  while(temp->next_page != NULL) {
    ++num;
    temp = temp->next_page;
  }
  printf("there are %d free pages.\n", num);

  /*  printf("allocate_free_page_test():try to allocate 2000 page\n");
  for (int i = 0; i < 2000; i++) {
    p = allocate_page();
    printf("allocate_free_page_test():page_num:%d page_count:%d\n", p->page_num, p->page_count);
  }

  temp = &free_page_list;
  num = 0;
  while(temp->next_page != NULL) {
    ++num;
    temp = temp->next_page;
  }
  printf("there are %d free pages.\n", num);*/

}

void no_page(int addr) {
  #ifdef DEBUG
  printf("page is not present(int addr):%b....\n", addr);
  #endif
}

void wp_page(int addr) {
  #ifdef DEBUG
  printf("page is only readable(int addr):%b....\n", addr);
  #endif
}

/* page fault handler. */
void page_fault_handler(struct registers *r) {
  uint addr;

  /*  asm volatile("movl %%cr2, %0":"=a"(addr));

  if ((r->err_code & 0x001)==0) {
    no_page(addr);
    //    return;
    //    return 0;
  }

  if (r->err_code & 0x002) {
    wp_page(addr);
    //    return;
    //    return 0;
  }
  */

  static unsigned int debug=1;

  int present   = (r->err_code & 0x1); // Page not present
  int write = r->err_code & 0x2;           // Write operation?
  int user = r->err_code & 0x4;           // Processor was in user-mode?
  int reserved = r->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = r->err_code & 0x10;          // Caused by an instruction fetch?

  if (!debug) {
    
    //    we must set the faulting address as allocated.

  } else {
    
    printf("Page Fault! bit:-----------------------------------\n", r->err_code);
    if (present) puts("present "); else puts("not present ");
    if (write) puts("write "); else puts("read ");
    if (user) puts("user-mode "); else puts("supervisor-mode ");
    if (reserved) puts("reserved ");
    
    puts("System Halted!---------------------------------------\n");

    //    for (;;)
    //      ;
  }
}

int page_table_copy(struct pde *des, struct pde *src) {
  struct pde * old_pde, *new_pde;
  struct pte * old_pt, *new_pt;
  struct pte * old_page, *new_page;
  struct page *p;

  // in virtual memory space, only the space after 'MEM_UPPER / 4096'KB is for user.
  // for that page directory.
  for (int i = MEM_UPPER / 4096; i < 1024; ++i) {
    new_pde = &src[i];
    old_pde = &des[i];
    new_pde->pd_flag = old_pde->pd_flag;

    // if that page directory is used, then we copy it.
    if (new_pde->pd_flag & 1) {
      old_pt = (struct pte*)(old_pde->pd_off * 4096);
      new_pt = (struct pte*)malloc(4096); // alloc a page table.
      new_pde->pd_off = ((unsigned int)new_pt) >> 12; // give value to the new page directory entry(point to a page table).

      // for that page talbe.
      for (int j = 0; j < 1024; ++j) {
        old_page = &old_pt[j];
        new_page = &new_pt[j];
        new_page->pt_off = old_page->pt_off;
        new_page->pt_flag = old_page->pt_flag;

    // if that page table is used, then we copy it.(in fact, we do not copy it, we make the two the same.
        if (old_page->pt_flag & 1) {
          new_page->pt_flag &= 0x1FD; // not writable
          old_page->pt_flag &= 0x1FD;
          p = &map[old_page->pt_off];
          p->page_count++;
        }
      }
    }
  }
  
  return 1;
}
