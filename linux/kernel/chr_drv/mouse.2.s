# 1 "mouse.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "mouse.S"
.globl mouse_interrupt
mouse_interrupt:
    pushl %eax
	pushl %ebx
	pushl %ecx 
	pushl %edx
	push %ds
	
    #prepare for call readmouse
	movl $0x10,%eax
	mov %ax,%ds
	
    
xor %eax,%eax
	inb $0x60,%al
	pushl %eax
	call readmouse
	addl $4,%esp



    #EOI
	movb $0x20,%al #8259A 操作命令字 EOI
	outb %al,$0xA0 #send EOI to 8259a second
	outb %al,$0x20 #send EOI to 8259a first
	
	
	pop %ds
	popl %edx
	popl %ecx
	popl %ebx
	popl %eax
	iret
