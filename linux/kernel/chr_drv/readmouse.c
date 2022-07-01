#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <a.out.h>

#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <asm/segment.h>

#include <sys/types.h>
#include <asm/io.h>

#define vga_graph_memstart 0xA0000
#define vag_graph_memsize 64000
#define cursor_side 3
#define vga_width 320
#define vga_height 200


static unsigned char mouse_input_count = 0;
static unsigned char mouse_left_down;
static unsigned char mouse_right_down;
static unsigned char mouse_left_move;
static unsigned char mouse_down_move;
static int mouse_x_position = 0;
static int mouse_y_position = 0;
static unsigned char mouse_x_overflow;
static unsigned char mouse_y_overflow;
void readmouse(int mousecode)
{
	//printk("mousecode: %d\n",mousecode);
	if(mousecode == 0xFA)
	{
		mouse_input_count = 1;
		return;
	}
	
	int offset = 0;
	switch(mouse_input_count)
	{
		case 1:
			mouse_left_down = (mousecode & 0x1) == 0x1;
			mouse_right_down = (mousecode & 0x2) == 0x2;
			mouse_left_move = (mousecode & 0x10) == 0x10;
			mouse_down_move = (mousecode & 0x20) == 0x20;
			mouse_x_overflow = (mousecode & 0x40) == 0x40;
            mouse_y_overflow = (mousecode & 0x80) == 0x80;
            mouse_input_count++;
            break;
        case 2:
            if(mouse_left_move)
               	offset = (int)(0xFFFFFF00|mousecode);
            else
                offset = (int)(mousecode);
			offset/=20;
			mouse_x_position+=offset;
            if(mouse_x_position < 0) mouse_x_position = 0;
			if(mouse_x_position > vga_width) mouse_x_position = vga_width;
            mouse_input_count++;
			//printk("Xpos: %d\n", mouse_x_position);
            break;
        case 3:
            if(mouse_down_move)
                offset = (int)(0xFFFFFF00|mousecode);
            else
                offset = (int)(mousecode);
			offset/=20;
			mouse_y_position+=offset;
            if(mouse_y_position < 0) mouse_y_position = 0;
			if(mouse_y_position > vga_height) mouse_y_position = vga_height;
            mouse_input_count = 1;
			//mouse_input_count++;
			//printk("Ypos: %d\n",mouse_y_position);
            break;
		// case 4:
		// 	mouse_input_count = 1;
		// 	break;
	}

	paintMouse();
}

void paintMouse()
{
	char *p = vga_graph_memstart;

	int i;
	for(i=0;i<vag_graph_memsize;i++)
		*(p+i) = 3;
	int j;
	for(i=mouse_x_position - cursor_side;i<=mouse_x_position + cursor_side; i++)
	{
		for(j=mouse_y_position-cursor_side;j<=mouse_y_position+cursor_side;j++)
		{
			if(i > 0 && j > 0 && i < vga_width && j < vga_height)
			{
				p = (char *)vga_graph_memstart+ j*vga_width+i;
				*p = 12;			
			}

		}
	}
}