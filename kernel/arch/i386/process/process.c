#include <process/process.h>

/* 
 * initialize the process, all the other processes will be clone from it.
 */
void process_init() {
  struct process *proc;

  // init the 'all_proc[64]'/'current_proc'
  all_proc[0] = &first_proc;
  current_proc = all_proc[0];
  proc = current_proc;

  proc->status = RUN;
  proc->id = 0;
  proc->parent_id = proc->id;
  proc->flag = LOAD;

  proc->priority = 0;
  proc->cpu_time = 0;

  proc->ms.page_dir = page_dir;

  tss.ss0 = 0x10;  // save the kernel data segment(0x10)
  tss.esp0 = 0x1000;  // stack, 4KB
}

/*
 * create a new process which got its own virtual memory space now, 
 * it is a little differnet from fork().
 */
struct process* create_kernel_proc(void (*func)(), int pri) {
  struct process* p;

  int empty_pid = 0; // we got to create a new process, and this is the index of the new process.

  // got empty_pid
  for(int i = 0; i < 16; i++) {
    if (all_proc[i] == NULL) {
      empty_pid = i;
      break;
    }
  }

  // no empty_pid, it means we got 16 process running at the same time already.
  if (empty_pid == 0) {
    printf("create_proc(void (*func)(), int pri):no empty_pid!\n");
    while(1) {}
  }

  // each process has a stack, 8KB.
  p = (struct process *)malloc(0x2000);

  // add the process to the processes array, but it is not running
  p->status = SLEEP;
  all_proc[empty_pid] = p;

  p->id = empty_pid;
  p->parent_id = current_proc->id; // its parent process
  p->flag = current_proc->flag;
  p->priority = pri; // low priority
  p->cpu_time = current_proc->cpu_time;
  
  // the virtual memory space and the context.
  //  ms_copy(&p->ms);
  p->ms.page_dir = current_proc->ms.page_dir;
  p->context = current_proc->context;
  p->context.ebx = (unsigned int)func;
  void * t = (void *)k_proc_entry;
  p->context.eip = t;
  p->context.esp = (unsigned int)p + 0x1000; // from the top to the buttom.
  p->status = RUN; // already to run now.

  #ifdef DEBUG1
  printf("create_proc(void (*func)(), int pri):the pid is:%d\n", empty_pid);
  printf("create_proc(void (*func)(), int pri):the pid in proc is:%d\n", p->id);
  #endif
  
  return p;
}


/*
 * to see if there is another process is going to run,
 * if yes, convert to that process.
 */
void polling() {
  //  int temp = 111;
  struct process *p = NULL, *next_p = NULL;

  /*  
  for (int i = 0; i < 16; ++i) {
    if (i == 16) i = 0;
    p = all_proc[i];
    if ((p) && (p->status == RUN || p->id == 0)) {
      if (p->priority <= temp) {
        next_p = p;
        temp = p->priority;
      }
    }
  }

  if (next_p == NULL) {
    next_p = all_proc[0];
    temp = p->priority;
  }
  */

  /*  if (current_proc == all_proc[0]) {
    printf("it is proc0 now!!!!!!!!!!!!!!!!!!!!\n");
    current_proc = all_proc[1];
  }

  if (all_proc[1] == current_proc) 
    next_p = all_proc[2];
  else if (all_proc[2] == current_proc)
    next_p = all_proc[1];
  else if (all_proc[0] == current_proc)
    next_p = all_proc[1];
  */

  /*  if (pp == 0) {

    pp = 1;
    next_p = all_proc[1];
    //    convert_to(next_p);

  } else if  (pp == 1) {

    pp = 2;
    next_p = all_proc[2];
  } else if  (pp == 2) {

    pp = 0;
    next_p = all_proc[0];
    //    convert_to(next_p);
  }

  if (current_proc->id = 15) {
    
  } else {
    while
  }
  */

  /*
  p = all_proc[(current_proc->id + 1) % 16];
  while (p) {
    if (p->status != ZOMB) {
      next_p = p;
      break;
    } else {
      p = all_proc[(p->id + 1) % 16];
    }
  }
  */

  int next_id = (current_proc->id + 1) % 16;
  for (int i = 0; i < 16; i++) {
    if (all_proc[next_id] == NULL || all_proc[next_id]->status == ZOMB) {
      next_id = (next_id + 1) % 16;
    } else {
      break;
    }
  }

  next_p = all_proc[next_id];

  //  #ifdef DEBUG
  //  printf("polling():will convert to pid:%d\n", next_p->id);
  //  #endif

  if (next_p->status != ZOMB)
    convert_to(next_p);

  //  #ifdef DEBUG
  //  printf("polling():convert to pid:%d over\n", next_p->id);
  //  #endif

}

void CC(struct context *src, struct context *des) {
  int a = 0x10;
  // does the actual context switch
  __asm__ ("cli"); // disable interrupts
  
  // 1. save the things to the task_curr
  
  __asm__ ("movl %%esp, %0;"
	   :"=r"(src->esp)        /* output */
	   );
  
  __asm__ ("movl %%ss, %0;"
	   :"=r"(a)
	   );

  //  task_curr = task_new; // change the task_curr pointer

  // 2. restore things from task_new

  __asm__ ("movl %0, %%esp;"
	   :
	   :"r" (des->esp)
	   );

  __asm__ ("movl %0, %%ss;"
	   :
	   :"r" (a)
	   );
  
  __asm__ ("sti"); // enable interrupts again
}

void convert_to(struct process *des){

  struct process *src;
  tss.esp0 = (uint)des + PAGE;
  src = current_proc;
  current_proc = des;

  /*
  #ifdef DEBUG
  printf("convert_to(struct process *des):try load page dir and convert--------------.\n");
  #endif

  // load page directory
  //  asm volatile("mov %%eax, %%cr3":: "a"((uint)des->ms.page_dir));

  #ifdef DEBUG
  printf("convert_to(struct process *des):try convert*************.\n");
  #endif
  */

  // start to convert
  convert(&(src->context), &(des->context));

}

void proc_exit() {
  current_proc->status = ZOMB;
  //  current_proc->exit_code = error_code;

  //  sti();
  polling();
}

int proc_num() {
  int n = 0;

  for(int i = 0; i < 16; i++) {
    if (all_proc[i] != NULL) {
      if (all_proc[i]->status != ZOMB)
        n++;
    }
  }

  return n;
}
