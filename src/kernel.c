#include "mini_uart.h"
#include "utils.h"
#include "spinlock.h"
#include "printf.h"
#include "timer.h"
#include "irq.h"
#include "fork.h"
#include "sched.h"
#include "sys.h"

void user_process1(char *array)
{
	char buf[2] = {0};
	while (1){
		for (int i = 0; i < 5; i++){
			buf[0] = array[i];
			call_sys_write(buf);
			delay(100000);
		}
	}
}

void user_process(){
	char buf[30] = {0};
	tfp_sprintf(buf, "User process started. EL 0\n\r");
	call_sys_write(buf);
	unsigned long stack = call_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	int err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"12345", stack);
	if (err < 0){
		printf("Error while cloning process 1\n\r");
		return;
	} 
	stack = call_sys_malloc();
	if (stack < 0) {
		printf("Error while allocating stack for process 1\n\r");
		return;
	}
	err = call_sys_clone((unsigned long)&user_process1, (unsigned long)"abcde", stack);
	if (err < 0){
		printf("Error while cloning process 2\n\r");
		return;
	} 
	call_sys_exit();
}

void kernel_process(){
	printf("Kernel process started. EL %d\r\n", get_el());
	int err = move_to_user_mode((unsigned long)&user_process);
	if (err < 0){
		printf("Error while moving process to user mode\n\r");
	} 
}

void kernel_main(int cpuId)
{
	
	if (cpuId == 0) 
	{
		int exec_lvl = get_el();
		uart_init(BAUD_115200);
		init_printf(0, putc);
		printf("\r\nBooting into Vosix v0.01!\r\n");
		printf("Current exception level: %d\r\n", exec_lvl);		
	}

	char cpuIdStr[2];
	cpuIdStr[0] = cpuId + '0';
	cpuIdStr[1] = 0;

	try_lock(cpuId);
	printf("This is processor: %s\r\n", cpuIdStr);
	close_lock();
	
	if (cpuId == 0) 
	{
		delay(10000000);
		irq_vector_init();
		timer_init();
		enable_interrupt_controller();
		enable_irq();		

		int res = copy_process(PF_KTHREAD, (unsigned long)&kernel_process, 0, 0, 1);
		if (res < 0) {
			printf("error while starting process 1");
			return;
		}

		while (1){
			schedule();
		}
	}	
	else
	{		
		while(1){}		
	}	
}
