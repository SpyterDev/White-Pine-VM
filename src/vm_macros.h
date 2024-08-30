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

#pragma once

#define allocated_memory 53248
#define stack_size 524288
#define less_than 1
#define greater_than 2
#define equal_to 4
#define all_cond 16
#define real_address_mask 0xfff
#define real_address_bits 12
#define stack_mem_pages (stack_size >> real_address_bits)
#define heap_mem_pages (allocated_memory / real_address_bits)
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
typedef uint8_t * arithmetic_void_ptr;