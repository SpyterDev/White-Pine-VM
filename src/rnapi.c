#include "instr.h"
#include "vm.h"
#include <stdio.h>
#include <stdint.h>

void rnapi_print(void) {
    uint32_t current_mem = r[decoded_flags.rM];
    register uint8_t current_char = 1;
    while (current_char) {
        current_char = * (uint8_t *) VirtualMemToReal(current_mem);
        printf("%c", current_char);
        current_mem += 1;
    }
}
struct {
    const uint16_t num_of_funcs;
    void (*func[0xffff])(void);
} runtime_native_api_functions = {1, {rnapi_print}};