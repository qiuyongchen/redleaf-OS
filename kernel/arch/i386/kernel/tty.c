#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
        //	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_color = make_color(COLOR_BLACK, COLOR_WHITE);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index_t = y * VGA_WIDTH + x;
			terminal_buffer[index_t] = make_vgaentry(' ', terminal_color);
		}
	}
	terminal_row = 1;
}

/*void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
*/

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar(char c)
{
  if (c == '\n') {
    terminal_column = 0;
    terminal_row++;
  } else {
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    terminal_column++;
  }

  if (terminal_column >=80) {
    terminal_column = 0;
    terminal_row++;
  }

  if (terminal_row == 25) {
    terminal_scroll();
  }
}

void terminal_write(const char* data, size_t size)
{
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_scroll() {
        // reserve one line for system.
        memcpy(terminal_buffer + 80, &terminal_buffer[2 * 80 + 0], 23 * 80 * 2);
        terminal_row = 24;
	for ( size_t x = 0; x < VGA_WIDTH; x++ )
	{
          const size_t index = terminal_row * VGA_WIDTH + x;
          terminal_buffer[index] = make_vgaentry(' ', terminal_color);
	}


}
