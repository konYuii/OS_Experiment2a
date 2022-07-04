#include<linux/sched.h>
#include<asm/segment.h>
#include<ex2.h>
struct message msg_que[1024];
unsigned int msg_que_fron = NULL, msg_que_rear = NULL;
void post_message(int type){
    if (msg_que_rear != msg_que_fron - 1) {
		struct message msg;
		msg.mid = type;
		msg.pid = current->pid;
		msg_que[msg_que_rear] = msg;
		msg_que_rear = (msg_que_rear + 1) % 1024;
	}
}
int sys_get_message(char *id) {
    struct message tmp;
	if(msg_que_rear == msg_que_fron){
		// put_fs_long(-1,&msg->mid);
		// put_fs_long(-1,&msg->pid);
		put_fs_long(tmp.mid,id);
		return;
	}
	
	tmp = msg_que[msg_que_fron];
	msg_que[msg_que_fron].mid = 0;
	msg_que_fron = (msg_que_fron + 1) % 1024;;
	put_fs_long(tmp.mid,id);
	// put_fs_long(current->pid,id);

	return tmp.mid;
}