#include "mm.h"
#include "sched.h"
#include "entry.h"
#include "logger.h"
#include "fork.h"
#include "utils.h"

int copy_process(unsigned long clone_flags, unsigned long fn, unsigned long arg, unsigned long stack, unsigned long priority)
{
	preempt_disable();
	struct task_struct *p, *prev;

	p = (struct task_struct *) get_free_page();
	if (!p)
		return -1;

	struct pt_regs *childregs = task_pt_regs(p);
	memzero((unsigned long)childregs, sizeof(struct pt_regs));
	memzero((unsigned long)&p->cpu_context, sizeof(struct cpu_context));

	if (clone_flags & PF_KTHREAD) {
		p->cpu_context.x19 = fn;
		p->cpu_context.x20 = arg;
	} else {
		struct pt_regs * cur_regs = task_pt_regs(current);
		*cur_regs = *childregs;
		childregs->regs[0] = 0;
		childregs->sp = stack + PAGE_SIZE; 
		p->stack = stack;
	}

	p->flags = clone_flags;
	p->priority = priority;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)childregs;
	int pid = nr_tasks++;
	p->pid = pid;
	p->next = 0;

	// Add task to the end of kernel_task linked list
	prev = kernel_task;
	while(prev->next)
		prev = prev->next;
	prev->next = p;
	
	// Logging
	char log_msg[30] = {0};
	tfp_sprintf(log_msg, "[PID %d] Addr: 0x%x\r\n", pid, p);
	log(log_msg, DEBUG);
	tfp_sprintf(log_msg, "Stack Pointer: 0x%x\r\n", p->cpu_context.sp);
	log(log_msg, INFO);
	tfp_sprintf(log_msg, "Program Counter: 0x%x\r\n", p->cpu_context.pc);	
	log(log_msg, INFO);

	preempt_enable();
	return pid;
}

int move_to_user_mode(unsigned long pc)
{
	struct pt_regs *regs = task_pt_regs(current);
	memzero((unsigned long)regs, sizeof(*regs));
	regs->pc = pc;
	regs->pstate = PSR_MODE_EL0t;
	unsigned long stack = get_free_page(); //alocate new user stack
	if (!stack) {
		return -1;
	}
	regs->sp = stack + PAGE_SIZE; 
	current->stack = stack;
	return 0;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk) {
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);
	return (struct pt_regs *)p;
}