#ifndef PROCESS_H
#define PROCESS_H

#include <memory/liballoc.h>
#include <memory/paging.h>
#include <memory/tss.h>

#define DEBUG 0

/* process status. */
#define SLEEP 1
#define WAIT  2
#define RUN   3
#define ZOMB  4
#define STOP  5

/* process flags. */
#define LOAD       0x1
#define SYS        0x2
#define LOCK       0x4
#define SWAP       0x8
#define TRC        0x10
#define WTED       0x20

extern int k_proc_entry();

/* memory space for a process, normally, it contains
 * the page directory or the page table.*/
struct memory_space {
  struct pde* page_dir;
};

/* process context, save the context of the process 
 * when another process is going to run.
 */
struct context {
  int eip;
  int esp;
  int ebx;
  int ecx;
  int edx;
  int esi;
  int edi;
  int ebp;
  unsigned int resver;
  //  int resve;
};

/* struct for a prcess.
 * contains:
 *          process status 
 *          process id
 *          process parent's id
 *          process flag
 *          process priority
 *          process cpu time slice
 *          process memory space
 *          process context
 *          process exit code(sent to parent process)
 *          process's return error from a syscall
 */
struct process {
  char                      status;
  int                       id;                // process's id
  int                       parent_id;         // process's parent's id
  char                      flag;              // process's flag,whether the process is sheduling or being swapped
  int                       priority;          // process's priority, 0 is the highest priority.
  int                       cpu_time;
  struct memory_space       ms;                // process's memory space, it contains the virtual memory etc.
  struct context            context;           // process's context, when change to another process, we got to save
                                               // current process's context.
  int                       exit_code;         // pass this code to its parent process.
  int                       ret_error;         // from system call
};

/* a pointer that always point to the current running process.*/
struct process *current_proc;

/* record all the processes, it means that we got at most 16 process at the same time. */
struct process *all_proc[16];

/* the parent process of all the other processes. */
struct process first_proc;

void process_init();

struct process* create_kernel_proc(void (*func)(), int pri);

void polling();

void convert_to(struct process *des);

int pp;

void proc_exit();

int proc_num();

#endif
