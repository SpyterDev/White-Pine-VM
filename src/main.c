#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "instr.h"

// Version stuff
#define version_major 0
#define version_minor 0
#define version_micro 1

// Important macros
typedef uint8_t * arithmetic_void_ptr;
#define allocated_memory 53248
#define real_address_mask 0xfff
#define real_address_bits 12
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
#define pc r[15]
// Registers
uint32_t r[num_of_registers];

void * sp;

// Instruction decoding variables
struct {
    uint8_t cdn: 4; // Condition
    uint8_t rN: 4; // Register Number
    uint8_t rD: 4; // Register Destination
    uint32_t rM: 4; // Register Modified
    uint32_t imm; // Immediate
    uint8_t immM: 1; // Immediate Mode
} decoded_flags;





void * VirtualMemToReal(uint32_t address) {
    static struct {
        uint8_t * address; 
        unsigned int offset;
    } page[allocated_memory / real_address_mask];
    if (page[0].address == NULL) for (uint16_t i = 0; i < allocated_memory / real_address_mask; i+=1) page[i].address = malloc(real_address_mask), page[i].offset = (uint64_t)page[i].address & real_address_mask;
    if ((address>>real_address_bits) > allocated_memory / real_address_mask) return page[0].address+page[0].offset;
    return page[(address>>real_address_bits)].address + page[(address>>real_address_bits)].offset +(address&real_address_mask);
}

// Fetch the next White Pine instruction
unsigned long int fetch(uint64_t * program_loc) {
    return program_loc[pc++];
}

// Decoding Function for a White Pine instruction
unsigned char decode(uint64_t instruction) {
    decoded_flags.rN = (instruction & register_num_mask) >> register_num_shift;
    decoded_flags.rD = (instruction & register_dest_mask) >> register_dest_shift;
    decoded_flags.cdn = (instruction & condition_mask) >> condition_shift;
    if ((instruction & immediate_mask) >> immediate_shift) decoded_flags.imm = instruction & immediate_mask, decoded_flags.immM = (instruction & immediate_mask) >> immediate_shift;
    else decoded_flags.rM = instruction & 0xf;
    return (instruction & opcode_mask) >> opcode_shift;
}

// Execute a White Pine instruction
void execute(unsigned char opcode) {
    switch (opcode)
    {
    case mov:
        r[decoded_flags.rD] = decoded_flags.immM ? decoded_flags.imm : r[decoded_flags.rM];
        break;
    case push:
        *(uint32_t *)sp = decoded_flags.immM ? decoded_flags.imm : r[decoded_flags.rM];
        sp = (uint32_t *) sp + 1;
        break;
    case pop:
        sp = (uint32_t *) sp - 1;
        r[decoded_flags.rD] = *(uint32_t *)sp;
        break;
    case add:
        uint32_t add_1 = r[decoded_flags.rD] == r[decoded_flags.rN] ? 0 : r[decoded_flags.rN];
        r[decoded_flags.rD] = decoded_flags.immM ? add_1 + decoded_flags.imm : add_1 + r[decoded_flags.rM];
        break;
    case sub:
        uint32_t sub_1 = r[decoded_flags.rD] == r[decoded_flags.rN] ? 0 : r[decoded_flags.rN];
        r[decoded_flags.rD] = decoded_flags.immM ? sub_1 - decoded_flags.imm : sub_1 - r[decoded_flags.rM];
        break;
    case mul:
        uint32_t mul_1 = r[decoded_flags.rD] == r[decoded_flags.rN] ? 0 : r[decoded_flags.rN];
        r[decoded_flags.rD] = decoded_flags.immM ? mul_1 * decoded_flags.imm : mul_1 * r[decoded_flags.rM];
        break;
    case divd:
        uint32_t divd_1 = r[decoded_flags.rD] == r[decoded_flags.rN] ? 0 : r[decoded_flags.rN];
        r[decoded_flags.rD] = decoded_flags.immM ? divd_1 / decoded_flags.imm : divd_1 / r[decoded_flags.rM];
        break;
    case b:
        pc = decoded_flags.imm;
        break;
    case ldr:
        r[decoded_flags.rD] = *(uint32_t*) VirtualMemToReal(decoded_flags.imm);
        break;
    case str:
        *(uint32_t *)VirtualMemToReal(decoded_flags.imm) = r[decoded_flags.rD];
        break;
    case rti:
        break;
    default:
        break;
    }
}

// White Pine process function

void white_pine_process(uint64_t * program_loc) {
    pc = 0xffffffff;
    uint8_t opcode = decode(*program_loc);
    while (opcode) {
        execute(opcode);
        opcode = decode(fetch(program_loc+pc));
    }
}

// Main code
int main(void) {
    printf("White Pine VM: %d.%d.%d\n", version_major, version_minor, version_micro);
    white_pine_process(0);
    return 0;
}