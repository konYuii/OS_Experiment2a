#include <linux/tty.h>
#include <linux/sched.h>
#include <asm/system.h>
#include <asm/io.h>

//Experiment2
static unsigned char mouse_input_count = 0;
static unsigned char mouse_left_down;
static unsigned char mouse_right_down;
static unsigned char mouse_left_move;
static unsigned char mouse_down_move;
static int mouse_x_position;
static int mouse_y_position;
static unsigned char mouse_x_overflow;
static unsigned char mouse_y_overflow;
void readmouse(int mousecode)
{
	printk("mousecode: %d\n",mousecode);
	if(mousecode == 0xFA)
	{
		mouse_input_count = 1;
		return;
	}

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
                mouse_x_position += (int)(0xFFFFFF00|mousecode);
            else
                mouse_x_position += (int)(mousecode);
            if(mouse_x_position < 0) mouse_x_position = 0;
            mouse_input_count++;
			printk("Xpos: %d\n", mouse_x_position);
            break;
        case 3:
            if(mouse_down_move)
                mouse_y_position += (int)(0xFFFFFF00|mousecode);
            else
                mouse_y_position += (int)(mousecode);
            if(mouse_y_position < 0) mouse_y_position = 0;
            mouse_input_count = 1;
			//mouse_input_count++;
			printk("Ypos: %d\n",mouse_y_position);
            break;
		// case 4:
		// 	mouse_input_count = 0;
		// 	break;
	}
}
