// ────────────────────────────────────────────────────────────────────────────
// Datatypes — basic scalar type aliases and small utility macros.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// Project-wide aliases for fixed-width integers and floating point (`u8`…
// `u64`, `i8`…`i64`, `f32`, `f64`) plus `Bool`, `Size`, `String` and `Any`,
// along with a few helper macros.
//
// Usage:
//
//   u32 count = 0;
//   f64 ratio = 0.5;
//   String name = "kolekto";
// ────────────────────────────────────────────────────────────────────────────

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

// Boolean (already in <stdbool.h>, but alias for consistency)
typedef bool    Bool;
typedef size_t  Size;
typedef void * const Any;

// Strings
typedef char const * String;

#define UNUSED(x) ((void)(x))

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

#define ASSERT_OR(cond) do { ASSERT(cond); } while (0); if(!(cond))

#endif // KOLEKTO_DATATYPES
