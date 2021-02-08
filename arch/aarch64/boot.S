.section "text"
.extern _start

.section "text.boot"
.global boot
boot:
    // Hang all non primary cpus
    mrs x0, mpidr_el1		
    and x0, x0,#0xFF
    cbnz x0, proc_hang

    // Move the stack pointer to 0x8000 and jump to kernel_main
    mov	x0, #0x8000
    mov sp, x0 
    b _start

proc_hang:
    wfe
    b proc_hang