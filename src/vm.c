/*
    MIT License

    Copyright (c) 2024 Nicholas A. Fraidakis

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "instr.h"
#include "vm_macros.h"
#include "rnapi.h"

// Version stuff
#define version_major 0
#define version_minor 0
#define version_micro 1

// Registers
uint32_t r[num_of_registers];
void * sp = 0;
arithmetic_void_ptr stack_mem_address = 0;
uint64_t pc = 0;
uint64_t lr = 0;
uint32_t psr = 0xffffffff;

// Instruction decoding variables
struct {
    uint8_t cdn: 4; // Condition
    uint8_t rN: 4; // Register Number
    uint8_t rD: 4; // Register Destination
    uint32_t rM: 4; // Register Modified
    uint32_t imm; // Immediate
    uint8_t immM: 1; // Immediate Mode
} decoded_flags;

// Converts a Virtual Memory address into a *physical address
void * VirtualMemToReal(uint32_t address) {
    static uint8_t * heap_page[heap_mem_pages]; // Init static array to hold physical address of heap pages
    static _Bool heap_pages_init = 0; // A variable to hold whether all the page address have been set to 0 (NULL), also this variable is set to false (0) by default
    register uint32_t address_header = address >> real_address_bits; // Holds the page number of the inputed virtual address
    register uint32_t address_header_shifted = address_header - stack_mem_pages; // Holds the page number of the inputed virtual address offset with the number of stack pages
    if (!heap_pages_init) { 
        for (uint32_t i = 0; i < heap_mem_pages; i+=1) heap_page[i] = 0;
        heap_pages_init++; 
    } // Inits all memory address in heap_page to 0 (NULL) at start as a placeholder value (only gets called once)
    if (stack_mem_pages > address) return stack_mem_address + address; // Inputed address is on the stack and converted
    if (!heap_page[address_header_shifted]) {
        heap_page[address_header_shifted] = malloc(real_address_mask); // Allocates physical memory on the heap to be used
        if (!heap_page[address_header_shifted]) return VirtualMemToReal(0); // Failed to allocate page on heap, returning base memory address of stack as a failsafe
    } // Inits the page to a real physical address
    if (address_header_shifted > heap_mem_pages) return VirtualMemToReal(0); // If the inputed address is greater than the amount of virtual memory, virtual address is treated like 0
    return heap_page[address_header_shifted] + (address & real_address_mask); // Inputed address is on the heap and converted
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
    if (~psr & decoded_flags.cdn) return; // If condition is false, instruction is skipped
    switch (opcode)
    {
    case mov:
        r[decoded_flags.rD] = decoded_flags.immM ? decoded_flags.imm : r[decoded_flags.rM];
        break;
    case push:
        *(uint32_t *)sp = decoded_flags.immM ? decoded_flags.imm : r[decoded_flags.rM];
        if (sp < stack_mem_address + stack_size) sp = (uint32_t *) sp + 1;
        break;
    case pop:
        if (sp > stack_mem_address) sp = (uint32_t *) sp - 1;
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
    case bl:
        lr = pc;
        pc = decoded_flags.imm;
        break;
    case ret:
        pc = lr;
        lr = 0;
        break;
    case ldr:
        r[decoded_flags.rD] = *(uint32_t*) VirtualMemToReal(decoded_flags.imm);
        break;
    case str:
        *(uint32_t *)VirtualMemToReal(decoded_flags.imm) = r[decoded_flags.rD];
        break;
    case rnapi:
        if (decoded_flags.rM < runtime_native_api_functions.num_of_funcs) runtime_native_api_functions.func[decoded_flags.rM]();
        break;
    case cmp: 
        psr = psr&(~0xf);
        uint32_t compared_value = decoded_flags.immM ? decoded_flags.imm : r[decoded_flags.rM];
        if (r[decoded_flags.rN] == compared_value) psr |= equal_to;
        else if (r[decoded_flags.rN] > compared_value) psr |= less_than;
        else if (r[decoded_flags.rN] < compared_value) psr |= greater_than;
        break;
    default:
        break;
    }
}

// White Pine process function

void white_pine_process(uint64_t * program_loc, uint64_t program_size) {
    unsigned char stack_mem[stack_size];
    sp = stack_mem;
    stack_mem_address = stack_mem;
    pc = 0xffffffff;
    uint8_t opcode = decode(*program_loc);
    while (opcode && program_loc + pc < program_loc + program_size) {
        execute(opcode);
        opcode = decode(fetch(program_loc + pc));
    }
}

// White Pine loader

void white_pine(int argc, char **argv) {
    printf("White Pine VM: %d.%d.%d\n", version_major, version_minor, version_micro);
    if (argc == 1) goto noargs; 
    uint64_t * exe_in_mem = NULL;
    unsigned int length;
    FILE * wp_exe_file = fopen(argv[1], "r");
    if (wp_exe_file) {
        fseek(wp_exe_file, 0, SEEK_END);
        length = ftell(wp_exe_file);
        fseek(wp_exe_file, 0, SEEK_SET);
        exe_in_mem = (uint64_t *)malloc(length);
        if (exe_in_mem) {
            fread(exe_in_mem, 1, length, wp_exe_file);
            white_pine_process(exe_in_mem, length);
        } else printf("An Error Occurred Allocating Program Memory");
        fclose(wp_exe_file);
        return;
    }
noargs:
    printf("Usage: white_pine [exe-path]");
}