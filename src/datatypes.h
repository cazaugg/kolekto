#ifndef KOLEKTO_DATATYPES
#define KOLEKTO_DATATYPES

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

// Unsigned integers
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed integers
typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating-point
typedef float  f32;
typedef double f64;

// Boolean (already in stdbool.h, but alias for consistency)
// typedef bool     bool
typedef size_t size;
typedef void * const any;

// Strings
typedef char const * string;

typedef struct
{
    char * const data;
    const u32 capacity;
    u32 length;
} string_builder;

#define NEW_STRING_BUILDER(len)     (string_builder){.data = (u8[len]){0}, .capacity = len, .length = 0}

#define UNUSED(x) (void)(x);

#define MIN(a, b)  __extension__({ \
        typeof_unqual(a) _a = (a); \
        typeof_unqual(b) _b = (b); \
        _a < _b ? _a : _b; \
    })     

#ifdef DEBUG
#define ASSERT(cond)   if(!(cond)){ while(1){}}
#else
#define ASSERT(cond)
#endif

#define ASSERT_OR(cond) do { assert(cond); } while (0); if(!(cond))

#endif // KOLEKTO_DATATYPES
