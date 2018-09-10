#include "mm.h"
#include "sched.h"
#include "entry.h"
#include "printf.h"

int copy_process(unsigned long fn, unsigned long arg, unsigned long priority)
{
	preempt_disable();
	struct task_struct *p, *prev;

	p = (struct task_struct *) get_free_page();
	if (!p)
		return 1;
	p->priority = priority;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail

	p->cpu_context.x19 = fn;
	p->cpu_context.x20 = arg;
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)p + THREAD_SIZE;
	int pid = nr_tasks++;
	p->pid = pid;
	p->next = 0;

	// Add task to the end of kernel_task linked list
	prev = kernel_task;
	while(prev->next)
		prev = prev->next;
	prev->next = p;
	
	printf("[PID %d] Addr: 0x%x\r\n", pid, p);
	printf("Stack Pointer: 0x%x\r\n", p->cpu_context.sp);
	printf("Program Counter: 0x%x\r\n", p->cpu_context.pc);	
	preempt_enable();
	return 0;
}