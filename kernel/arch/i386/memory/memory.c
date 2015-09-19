#include <memory/memory.h>

void memory_init(multiboot_info_t *mb) {
  printf("memory status:\n");
  printf("lower:%d\n", mb->mem_lower);
  printf("upper:%d\n", mb->mem_upper);
  printf("the size in KB:%d\n", mb->mem_lower + mb->mem_upper);

  get_memory_size(mb);

  paging_init();

  page_dir_init(page_dir);

  set_page_fault_handler();

  // load page directory
  asm volatile("mov %%eax, %%cr3":: "a"((uint)page_dir));

  mmu_enable();

  //allocate_free_page_test();

  //allocate_test();
}

void get_memory_size(multiboot_info_t *mb) {
  MEM_UPPER = mb->mem_upper;
  MEM_LOWER = mb->mem_lower;

  /* get the size of physical memory, add the first 1 megabyte(1024 bytes)*/
  MEM_SIZE = MEM_UPPER + 1024;
}

void allocate_test() {
  struct page* temp;
  int addr;

  int num = 0;
  struct page *temp_for_count = &free_page_list;

  #ifdef DEBUG
  temp_for_count = &free_page_list;
  num = 0;
  while(temp_for_count->next_page != NULL) {
    ++num;
    temp_for_count = temp_for_count->next_page;
  }
  printf("there are %d free pages.\n", num);
  #endif

  /* try to malloc for the first time. */
  printf("allocate_test():try allocate\n");

  temp = (void *)malloc(sizeof(struct page) * 10);

  printf("allocate_test():allocate successful...begin addr:%d---end addr:%d\n", temp, temp + sizeof(struct page) * 10);

  #ifdef DEBUG
  temp_for_count = &free_page_list;
  num = 0;
  while(temp_for_count->next_page != NULL) {
    ++num;
    temp_for_count = temp_for_count->next_page;
  }
  printf("there are %d free pages.\n", num);
  #endif

  /* try to free for the first time. */
  printf("allocate_test():try free\n");
  
  free(temp);

  printf("allocate_test():free successful...\n");

  #ifdef DEBUG
  temp_for_count = &free_page_list;
  num = 0;
  while(temp_for_count->next_page != NULL) {
    ++num;
    temp_for_count = temp_for_count->next_page;
  }
  printf("there are %d free pages.\n", num);
  #endif

  /* try to malloc for the second time. */
  printf("allocate_test():try allocate again\n");

  temp = (void *)malloc(sizeof(struct page) * 10);

  printf("allocate_test():allocate successful...begin addr:%d---end addr:%d\n", temp, temp + sizeof(struct page) * 10);

  #ifdef DEBUG
  temp_for_count = &free_page_list;
  num = 0;
  while(temp_for_count->next_page != NULL) {
    ++num;
    temp_for_count = temp_for_count->next_page;
  }
  printf("there are %d free pages.\n", num);
  #endif

  /* try to free for the second time. */
  printf("allocate_test():try free again\n");
  
  free(temp);

  printf("allocate_test():free successful...\n");

  #ifdef DEBUG
  temp_for_count = &free_page_list;
  num = 0;
  while(temp_for_count->next_page != NULL) {
    ++num;
    temp_for_count = temp_for_count->next_page;
  }
  printf("there are %d free pages.\n", num);
  #endif

}

/*int ms_copy(struct memory_space *des) {

  // allocate memory for the page directory.
  //  des->page_dir = (struct pde*)malloc(4096);
  //  des->page_dir = page_dir;

  //  page_dir_init(des->page_dir);

  // page_table_copy(des->page_dir, current_proc->ms.page_dir);

  return 1;
}*/
