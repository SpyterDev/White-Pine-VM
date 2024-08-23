#pragma once

enum instr_set {
    halt, 
    ldr, str, // Copying or loading values into registers
    push, pop, // Stack commands
    add, // Adding
    sub, // Subtracting
    mul, // Multipling
    divd, // Dividing
    lsl, lsr, // Bit shifting
    rti, // Runtime interrupt
    b, // Branch
    extend, // Extends a 8 or 16 bit number to a 32 bit number
    compress, // Compress a 32 bit number to a 8 or 16 bit number
    _section,
    mov,
    num_of_instrs
};