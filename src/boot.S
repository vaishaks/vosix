#include "arm/sysregs.h"
#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
//	cbz x0, init_mem		//=dbg
	cbz	x0, switch_to_el1	//~dbg
	b	init_stack			// Other procs jump to stack init

switch_to_el1:
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

	adr	x0, init_mem		
	msr	elr_el3, x0

	eret				

init_mem:
	adr	x0, bss_begin		// Get bss start address
	adr	x1, bss_end
	sub	x1, x1, x0			// Compute size of bss
	bl 	memzero				// Initialize memory to zero
	b init_stack

init_stack:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Get CPUID
	mov x1, #STACK_SPACE	// Get stack size
	mul x1, x1, x0			// Multiply with CPUID to get start location of stack
	add x1, x1, #LOW_MEMORY // Add stack offset to starting memory location
	mov sp, x1				// Set stack pointer to starting location
	bl kernel_main			// Start kernel
	b  proc_hang

proc_hang: 
	b 	proc_hang			// Should never come here
