#pragma once
#include <stdint.h>

void rnpi_print(void);
extern const uint16_t number_of_rnapi_functions;
extern void (*runtime_native_api_functions[0xffff])(void);