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

enum instr_set {
    halt, 
    ldr, str, // Copying or loading values into registers
    push, pop, // Stack commands
    add, // Adding
    sub, // Subtracting
    mul, // Multipling
    divd, // Dividing
    lsl, lsr, // Bit shifting
    rnpi, // Runtime native api
    b, // Branch
    extend, // Extends a 8 or 16 bit number to a 32 bit number
    compress, // Compress a 32 bit number to a 8 or 16 bit number
    _section, // A header to state whether the follow is an instruction or otherwise
    mov, // Moving values into or from the general-purpose registers
    cmp, // Compare RN with RM, output is in bits 0-3 in PSR (Program Status Register)
    num_of_instrs
};