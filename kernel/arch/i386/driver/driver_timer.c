#include <driver/driver_timer.h>

int num = 0;
unsigned int timer_ticks=0;
unsigned int timer_sec=0;
unsigned int timer_min=0;
unsigned int timer_hrs=0;

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

void timer_handler3(struct registers *r)
{
  timer_ticks++;
  if ((timer_ticks%100==0)&&(timer_ticks!=0)) {timer_ticks=0;timer_sec++;}
  if ((timer_sec%60==0)&&(timer_sec!=0)) {timer_sec=0;timer_min++;}
  if ((timer_min%60==0)&&(timer_min!=0)) {timer_min=0;timer_hrs++;}

  num++;
  //  printf("num:%d\n", num);

  //  #ifdef DEBUG
  //  printf("hour:%d min:%d sec:%d\n", timer_hrs, timer_min, timer_sec);
  //  #endif

  sti();
}

void timer_init() {
  irq_install_handler(0, &timer_handler);
  //  irq_install_handler(1, &timer_handler2);

  // every 10ms, there is a timer interrupt(100HZ)
  timer_phase(100);

  // enable timer interrupt.
  //  irq_set_mask(0);

  //  printf("timer_init():had enable timer irq\n");
 
  num = 0;
  timer_ticks=0;
  timer_sec=0;
  timer_min=0;
  timer_hrs=0;

  get_system_time();
  update_time();
}

void timer_handler(struct registers *r)
{
  timer_ticks++;
  if ((timer_ticks%100==0)&&(timer_ticks!=0)) {
    timer_ticks=0;
    timer_sec++;

    /* sheculing the processes...*/
    //    printf("hour:%d min:%d sec:%d\n", timer_hrs, timer_min, timer_sec);

    update_time();
  }

  if ((timer_sec%59==0)&&(timer_sec!=0)) {timer_sec=0;timer_min++;}
  if ((timer_min%59==0)&&(timer_min!=0)) {timer_min=0;timer_hrs++;}

  num++;

  //  printf("num:%d", num);
  //  #ifdef DEBUG
  //  #endif
  // timer_handler2(r);
  /* every 100ms.*/

  sti();
  if (timer_ticks % 2 == 0)
    polling();
}

void update_time() {
    terminal_putentryat((timer_hrs / 10) + '0', 0xF0, 72, 0);
    terminal_putentryat((timer_hrs % 10) + '0', 0xF0, 73, 0);
    terminal_putentryat(':', 0xF0, 74, 0);
    terminal_putentryat((timer_min / 10) + '0', 0xF0, 75, 0);
    terminal_putentryat((timer_min % 10) + '0', 0xF0, 76, 0);
    terminal_putentryat(':', 0xF0, 77, 0);
    terminal_putentryat((timer_sec / 10) + '0', 0xF0, 78, 0);
    terminal_putentryat((timer_sec % 10) + '0', 0xF0, 79, 0);
}

void get_system_time() {
  // get hours ( +8 ).
  timer_hrs = get_from_chip(4) + 8;
  // get minutes.
  timer_min = get_from_chip(2);
  // get seconds.
  timer_sec = get_from_chip(1);
}

int get_from_chip(char a) {
    outb(0x70, a&0x7F);
    char temp = inb(0x71);
    return (temp & 15) + (temp >> 4) * 10;
}
