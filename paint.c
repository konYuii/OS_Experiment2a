#define __LIBRARY__
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<time.h>
#include<linux/tty.h>
#define vga_graph_memstart 0xA0000
#define vga_graph_memsize 64000
#define cursor_side 4
#define vga_width 320
#define vga_height 200


#define __NR_init_graphics 92
#define __NR_get_message 93
#define __NR_paint 94
#define __NR_timercreate 95

#define MSG_MOUSE_LEFT_DOWN 1
#define MSG_MOUSE_RIGHT_DOWN 2




_syscall0(int,init_graphics)
_syscall1(int,get_message,long *,id)
_syscall3(int,paint,long ,x,long, y,long, color)

int main()
{
	long i,j;
	long *id = 0;

	init_graphics();

	while(1)
	{

		get_message(id);

		if(*id == MSG_MOUSE_LEFT_DOWN)
		{
			for(i=0;i<vga_width;i++)
			{
				for(j=0;j<vga_height;j++)
				{
					paint(i,j,12);
				}
			}
		}
		if(*id == MSG_MOUSE_RIGHT_DOWN)
		{
			for(i=0;i<vga_width;i++)
			{
				for(j=0;j<vga_height;j++)
				{
					paint(i,j,3);
				}
			}
		}

	}

	return 0;
}

