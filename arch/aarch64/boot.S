.extern __start

.section "text.boot"
.global boot
boot:
    // Hang all non primary cpus
	mrs x0, mpidr_el1		
	and x0, x0,#0xFF
	cbnz x0, proc_hang

    // Set the stack
    mv sp, kernel_stack
    bl __start

proc_hang:
    wfe
    b proc_hang