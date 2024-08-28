#pragma once
#include <stdint.h>

// Important macros
typedef uint8_t * arithmetic_void_ptr;
#define allocated_memory 53248
#define stack_size 524288
#define less_than 1
#define greater_than 2
#define equal_to 4
#define all_cond 16
#define real_address_mask 0xfff
#define real_address_bits 12
#define stack_mem_pages stack_size >> real_address_bits
#define heap_mem_pages allocated_memory / real_address_bits
#define num_of_registers 16
#define condition_mask 0xF000000000000000 // Bits 63-61
#define condition_shift 60
#define immediate_mask 0x800000000000000 // Bits 60
#define immediate_shift 59
#define opcode_mask 0x7F8000000000000 // Bits 59-52
#define opcode_shift 51
#define set_mask 0x4000000000000 // Bits 51
#define set_shift 50
#define register_num_mask 0x3C00000000000 // Bits 47-44
#define register_num_shift 46
#define register_dest_mask 0x3C0000000000 // Bits 43-41
#define register_dest_shift 42
#define opperand_2_mask 0xffffffff // Bits 31-0

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
void * VirtualMemToReal(uint32_t address);
void white_pine(int argc, char **argv);