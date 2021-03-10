.macro save_registers_to_stack
    sub    sp, sp, #256
    stp    x0, x1, [sp, #16 * 0]
    stp    x2, x3, [sp, #16 * 1]
    stp    x4, x5, [sp, #16 * 2]
    stp    x6, x7, [sp, #16 * 3]
    stp    x8, x9, [sp, #16 * 4]
    stp    x10, x11, [sp, #16 * 5]
    stp    x12, x13, [sp, #16 * 6]
    stp    x14, x15, [sp, #16 * 7]
    stp    x16, x17, [sp, #16 * 8]
    stp    x18, x19, [sp, #16 * 9]
    stp    x20, x21, [sp, #16 * 10]
    stp    x22, x23, [sp, #16 * 11]
    stp    x24, x25, [sp, #16 * 12]
    stp    x26, x27, [sp, #16 * 13]
    stp    x28, x29, [sp, #16 * 14]
    str    x30, [sp, #16 * 15] 
.endm

.macro get_registers_from_stack
    ldp    x0, x1, [sp, #16 * 0]
    ldp    x2, x3, [sp, #16 * 1]
    ldp    x4, x5, [sp, #16 * 2]
    ldp    x6, x7, [sp, #16 * 3]
    ldp    x8, x9, [sp, #16 * 4]
    ldp    x10, x11, [sp, #16 * 5]
    ldp    x12, x13, [sp, #16 * 6]
    ldp    x14, x15, [sp, #16 * 7]
    ldp    x16, x17, [sp, #16 * 8]
    ldp    x18, x19, [sp, #16 * 9]
    ldp    x20, x21, [sp, #16 * 10]
    ldp    x22, x23, [sp, #16 * 11]
    ldp    x24, x25, [sp, #16 * 12]
    ldp    x26, x27, [sp, #16 * 13]
    ldp    x28, x29, [sp, #16 * 14]
    ldr    x30, [sp, #16 * 15] 
    add    sp, sp, #256        
.endm

entry: 
    save_registers_to_stack

    // Jump into C++ with info about exceptions
    // parameter 1 previously set in entry for each type 
    // of exception
    mrs x1, esr_el1
    mrs x2, elr_el1
    bl exception_entry

    get_registers_from_stack

    eret

.macro ventry_sync
    .align 7
    mov x0, #0
    b entry
.endm

.macro ventry_int
    .align 7
    mov x0, #1
    b entry
.endm

.macro ventry_fint
    .align 7
    mov x0, #2
    b entry
.endm

.macro ventry_serr
    .align 7
    mov x0, #3
    b entry
.endm

.macro ventry_inv
    .align 7
    eret
.endm

.align 11
__vectors:
    ventry_sync      // Synchronous EL1t
    ventry_int       // IRQ EL1t
    ventry_fint      // FIQ EL1t
    ventry_serr      // Error EL1t

    ventry_sync      // Synchronous EL1h
    ventry_int       // IRQ EL1h
    ventry_fint      // FIQ EL1h
    ventry_serr      // Error EL1h

    ventry_inv       // Synchronous 64-bit EL0
    ventry_inv       // IRQ 64-bit EL0
    ventry_inv       // FIQ 64-bit EL0
    ventry_inv       // Error 64-bit EL0

    ventry_inv       // Synchronous 32-bit EL0
    ventry_inv       // IRQ 32-bit EL0
    ventry_inv       // FIQ 32-bit EL0
    ventry_inv       // Error 32-bit EL0

.global setup_vector_table
setup_vector_table:
    adr x0, __vectors
    msr vbar_el1, x0 
    ret