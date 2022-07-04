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

typedef struct{
	int x;
	int y;
}point;

point t[3];


inline void swap(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void Bresenham_Line (point p1, point p2)
{
	int ix,iy,d,x,y;

    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);

    int direction = 1;
    if (dx < dy)
    {
        direction = 0;
        swap(&p1.x, &p1.y);
        swap(&p2.x, &p2.y);
        swap(&dx, &dy);
    }
    ix = (p2.x - p1.x) > 0 ? 1 : -1;
	iy = (p2.y - p1.y) > 0 ? 1 : -1;
    d = dy * 2 - dx;
    x = p1.x;
    y = p1.y;
    if (direction)
    {
        while (x != p2.x)
        {
            if (d < 0)
            {
                d += dy * 2;
            }
            else
            {
                d += (dy - dx) * 2;
                y+=iy;
            }
			paint((long)x,(long)y,8);
            x+=ix;
        }
    }
    else
    {
        while (x != p2.x)
        {
            if (d < 0)
            {
                d += dy * 2;
            }
            else
            {
                d += (dy - dx) * 2;
                y+=iy;
            }
			paint((long)y,(long)x,8);
            x+=ix;
        }
    }


}


int main()
{
	long i,j;
	long *id = 0;
	t[0].x = 50;
	t[0].y = 150;
	t[1].x = 100;
	t[1].y = 50;
	t[2].x = 150;
	t[2].y = 150;


	init_graphics();

	while(1)
	{
		*id = 0;
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
			for(i=0;i<3;i++)
			{
				Bresenham_Line(t[i],t[(i+1)%3]);
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

