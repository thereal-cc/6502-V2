#ifndef UTIL_H
#define UTIL_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;

// CPU Defines
#define MEMORY_MAP_SIZE 0x10000
#define INTERRUPT_HANDLER 0xFFFA
#define RESET_LOW 0xFFFC
#define RESET_HIGH 0xFFFD
#define BRK_HANDLER 0xFFFE

// Status flag masks
#define UNUSED_FLAG   0x00
#define CARRY_FLAG    0x01
#define ZERO_FLAG     0x02
#define INTERRUPT_FLAG 0x04
#define DECIMAL_FLAG  0x08
#define BREAK_FLAG    0x10
#define OVERFLOW_FLAG 0x40
#define NEGATIVE_FLAG 0x80

#endif