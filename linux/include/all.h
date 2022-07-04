#ifndef __ALL_H__
#define __ALL_H__


struct pho {
    long color;
    long x;
    long y;
    long dx;
    long dy;
};
struct user_timer{
	long init_jiffies;
	long jiffies;
	int type; // 1为一次闹钟
			  // 0为无数次闹钟
	int pid; // 哪个进程创建的定时器
	struct user_timer * next;
};
struct message{
    long mid; 
    long pid; //当前进程为-1
};
extern struct message msg_que[1024];
extern void post_message(int type);

// mid取值
#define MSG_MOUSE_LEFT_DOWN 1
#define MSG_MOUSE_RIGHT_DOWN 2
#define MSG_MOUSE_CENTER_DOWN 3
#define MSG_USER_TIMER 4
#define TYPE_USER_TIMER_INFTY 5
#define TYPE_USER_TIMER_ONCE 6

#endif 