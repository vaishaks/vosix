#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master			// CPUID 0x0 does bss init 
	b	init_stack			// Other procs jump to stack init

master:
	adr	x0, bss_begin		// Get bss start address
	adr	x1, bss_end
	sub	x1, x1, x0			// Compute size of bss
	bl 	memzero				// Initialize memory to zero

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
