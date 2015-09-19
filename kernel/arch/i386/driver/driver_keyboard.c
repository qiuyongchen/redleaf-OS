#include <driver/driver_keyboard.h>

extern int character[999];

extern int index;

extern int enable;

static uint mode = 0;

void keyboard_init() {
  irq_install_handler(1, &keyboard_handler);
}

void keyboard_handler(struct registers *r) {
    char sc, ch, m;
    char *map = keybd_map;

    if ((inb(0X64) & 0X1)==0){
        return -1;
    }
    sc = inb(0X60);

    if ((sc & 0x7f) == 0x3A)
        return 0;

    if (sc == 0xE0)
        mode |= 0X8;

    if ((m = shift(sc))) {
        if (sc & 0x80)
            mode &= ~m;
        else
            mode |= m;
        return 0;
    }
    map = (mode & 0X1)? shift_map: keybd_map;
    ch = map[sc & 0x7f];

    if (mode & 0X2) {
        switch(ch){
        case 'c': ch = 28;
        case 'd': ch = 04;
        case 'x': ch = 25;
        case 'q': ch = 21;
        case 's': ch = 23;
        case 'z': ch = 32;
        case '\\': ch = 34;
        }
    }

    if ((sc & 0x80)==0 && ch!='\0') {
      //        tty_input(&tty[0], ch);
      if (enable != 0) {
        character[++index] = ch;
        printf("%c", ch);
        if (ch == '\n')  {
          enable = 0;
        }
      } else {
        printf("not enable, input %c fail...\n", ch);
      }
    }
    else {
        mode &= ~0X8;
    }

    return 0;

}

char shift(char sc){
    char ch = sc & 0x7f;

    if (mode & 0X8) {
        switch (ch) {
        case 0x1D: return 0X2;
        case 0x38: return 0X4;
        }
    }
    else {
        switch(ch) {
        case 0x2A:
        case 0x36: return 0X1;
        case 0x1D: return 0X2;
        case 0x38: return 0X4;
        }
    }
    return 0;
}

