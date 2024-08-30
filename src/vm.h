#pragma once
#include <stdint.h>
#include "vm_macros.h"

// Instruction decoding variables
extern struct {
    uint8_t cdn: 4; // Condition
    uint8_t rN: 4; // Register Number
    uint8_t rD: 4; // Register Destination
    uint32_t rM: 4; // Register Modified
    uint32_t imm; // Immediate
    uint8_t immM: 1; // Immediate Mode
} decoded_flags;
// Registers
extern uint32_t r[num_of_registers];
extern void * sp;
extern arithmetic_void_ptr * stack_mem_address;
extern uint64_t pc;
extern uint32_t psr;

// Functions
extern void * VirtualMemToReal(uint32_t address);
extern void white_pine(int argc, char **argv);