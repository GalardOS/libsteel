.section ".text.boot"

.globl _boot
_boot:
    mrs    x0, mpidr_el1        
    and    x0, x0,#0xFF          // Check processor id
    cbnz    x0, proc_hang        // Hang for all non-primary CPU
    
    /// TODO: check if running execution level and jump to el1 from
    ///       any of them 
    // Enable eneric arm timers for el1
start_el2:
    cmp     x0, #4
    beq     start_el2
    msr     sp_el1, x1
    // enable CNTP for EL1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov     x0, #(1 << 31)      // AArch64
    orr     x0, x0, #(1 << 1)   // SWIO hardwired on Pi3
    msr     hcr_el2, x0
    mrs     x0, hcr_el2
    // change execution level to EL1
    mov     x2, #0x3c4
    msr     spsr_el2, x2
    adr     x2, start_el1
    msr     elr_el2, x2
    eret

start_el1:
    // Enable sharing stack with el0
    mov x0, #1
    msr SPSel, x0

    // Setup the stack 
    adr x0, _boot
    mov sp, x0
    bl gimme_all_your_lovin
    b proc_hang        // should never come here

proc_hang: 
    b     proc_hang