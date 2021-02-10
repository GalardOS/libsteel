#include "mm.h"

.section ".text.boot"

.globl _boot
_boot:
	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbnz	x0, proc_hang		// Hang for all non-primary CPU
	
	mov	sp, #0x8000
	bl	gimme_all_your_lovin
	b 	proc_hang		// should never come here

proc_hang: 
	b 	proc_hang

master:
