#define __LIBRARY__
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/segment.h>
#include <sys/stat.h>

#include <fcntl.h>

#define BUF_MAX 4096
#define	DIRBUF		8192
#define NAME_MAX          14

struct dirent {
    long           d_ino;
    off_t          d_off;
    unsigned short d_reclen;
    char           d_name[NAME_MAX+1];
};

/*参数count指定该缓冲区的大小*/
int sys_getdents(unsigned int fd,struct linux_dirent *dirp,unsigned int count)
{
    if(!count)return -1;/*count is zero*/
    if(fd>=NR_OPEN)return -EINVAL;/*fd is over range*/
    
    struct file *file;
    struct m_inode * inode;
    int ret;
    struct buffer_head *hd;
    struct dir_entry *de;
    struct dirent * temp;
    char * buf;
    int desize,ldsize,i,ldi;

    file=current->filp[fd];
    if(!file)return ENOTDIR;/*the file is not exist or not file*/
    ldsize = sizeof(struct dirent);
    desize = sizeof(struct dir_entry);
    inode = file ->f_inode;
    temp = (struct dirent *)malloc(ldsize);/* //the inter veriable */
    buf = (char*)malloc(ldsize); 

    /*get the inode's bread*/
    hd = bread(inode->i_dev , inode->i_zone[0]);

    ldi=0;
    ret=0;

    for (;ret<inode->i_size;ret += desize){
        if (ldi >= count-ldsize)
            break; /* full */
        de = (struct dir_entry *)(hd->b_data + ret);/* de is set to the current dir_entry */
        if (!de -> inode )/* to skip if there is no data in de */
            continue;            
        /*To  write, copying  current  dirent,  */
        temp->d_ino = de->inode;
        temp->d_off = 0; 
        temp->d_reclen = ldsize;
        strcpy(temp->d_name,de->name);

        
        /* by put_fs_byte to write back data to the usr */
        memcpy(buf, temp, ldsize);
        for (i=0;i < ldsize;i++){
            put_fs_byte(*(buf+i), ((char*)dirp)+i+ldi); 
        }
        /* memcpy(temp, buf, ldsize); */
        ldi += ldsize;
    }
    return ldi;
}






int sys_sleep(unsigned int seconds)
{
    sys_signal(SIGALRM,SIG_IGN);
    sys_alarm(seconds);
    if(sys_pause()!=-1)
    {
       /* printk("sleep\n");*/
        return 0;
    }
    
    
    /*printk("do not sleep\n");*/
    return -1;
}
/*just full the sys_call_table[89]*/
int sys_something()
{
    return 0;
}

typedef struct
	{
	int	dd_fd;			/* file descriptor */
	int	dd_loc;			/* offset in block */
	int	dd_size;		/* amount of valid data */
	char	*dd_buf;		/* -> directory block */
	}	DIR;			/* stream data from opendir() */
struct direct {
	ino_t d_ino;
	char d_name[NAME_MAX];
};

long sys_getcwd(char * buf, size_t size)
{
	struct m_inode *n_inode,*f_inode;
	struct buffer_head *block;
	unsigned int i_start;
	struct dir_entry *dir, *p_dir;
	char *tmp, *t_buf;
	t_buf = (char*)malloc(256);
	strcpy(t_buf, "");

	int len_dir = sizeof(struct dir_entry);
	/**/
	n_inode = current->pwd;
	if(n_inode == current->root) strcpy(t_buf, "/");

	while(n_inode != current->root)
	{
		block = bread(n_inode->i_dev, n_inode->i_zone[0]);
		dir = (struct dir_entry *)(block->b_data + len_dir);
		f_inode=iget(n_inode->i_dev, dir->inode);
		brelse(block);
		block = bread(f_inode->i_dev, f_inode->i_zone[0]);
		int k = 0;
		p_dir = (struct dir_entry *)(block->b_data + k);
		while(p_dir->inode)
		{
			if(p_dir->inode == n_inode->i_num)
			{
				strcpy(tmp, "/");
				strcat(tmp, p_dir->name);
				strcat(tmp, t_buf);
				strcpy(t_buf, tmp);
				break;
			}
			k += len_dir;
			p_dir = (struct dir_entry *)(block->b_data + k);
			
		}
		n_inode = f_inode;
		brelse(block);
	}
	if(strlen(t_buf)>size) return NULL;
	int i;
	for(i = 0; t_buf[i]; i++)
	{
		put_fs_byte(t_buf[i], buf + i);
	}
	return (long)buf;

}