#include <memory/liballoc_f.h>

/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
int liballoc_lock() {
  cli();
}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
int liballoc_unlock() {
  sti();
}

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
void* liballoc_alloc(int n) {
  struct page* p;
  for (int i = 1; i < n; ++i) {
    p = allocate_page();
    //    printf("page_num:%d-----", p->page_num);
  }
  p = allocate_page();
  //  printf("liballoc_alloc(int n):finish...page_num:%d.allocate finish, got to return\n", p->page_num);
  return (p->page_num * 4096);
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void* p,int n ) {
  printf("liballoc_free(void* p,int n): enter the f\n");
  struct page* page;
  for (int i = 0; i < n; ++i) {
    page = &map[(int)p / 4096];
    //    printf("liballoc_free(void* p,int n):will free a page, page_num:%d\n", page->page_num);
    free_page(page);
    p += 4096;
  }

  printf("liballoc_free(void* p,int n):free finish, got to return\n");
  return 0;
}
