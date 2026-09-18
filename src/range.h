#ifndef KOLEKTO_RANGE
#define KOLEKTO_RANGE

#include "datatypes.h"   

// Ranges allow to check if a value is within certain boundaries. There is a 
// specific range type for every basic number type, however the functions and 
// checks are generic.

// ────────────────────────────────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────────────────────────────────

// Unsigned
typedef struct {
    u8  min;
    u8  max;
} RangeU8;

typedef struct {
    u16 min;
    u16 max;
} RangeU16;

typedef struct {
    u32 min;
    u32 max;
} RangeU32;

typedef struct {
    u64 min;
    u64 max;
} RangeU64;

// Signed
typedef struct {
    i8  min;
    i8  max;
} RangeI8;

typedef struct {
    i16 min;
    i16 max;
} RangeI16;

typedef struct {
    i32 min;
    i32 max;
} RangeI32;

typedef struct {
    i64 min;
    i64 max;
} RangeI64;

// Floating Point
typedef struct {
    f32 min;
    f32 max;
} RangeF32;

typedef struct {
    f64 min;
    f64 max;
} RangeF64;

#define NEW_RANGE(min, max)  {min, max}

// ────────────────────────────────────────────────────────────────────────────
// Public Functions
// ────────────────────────────────────────────────
#define range_is_empty(r)          ((r).min > (r).max)

#define range_contains(range, value) \
    _Generic((range),                \
        RangeU8:  range_contains_u8,  \
        RangeU16: range_contains_u16, \
        RangeU32: range_contains_u32, \
        RangeU64: range_contains_u64, \
        RangeI8:  range_contains_i8,  \
        RangeI16: range_contains_i16, \
        RangeI32: range_contains_i32, \
        RangeI64: range_contains_i64, \
        RangeF32: range_contains_f32, \
        RangeF64: range_contains_f64  \
    )((range), (value))

    
// ────────────────────────────────────────────────────────────────────────────
// Implementation
// ────────────────────────────────────────────────
static inline Bool range_contains_u8 (RangeU8  r, u8  v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_u16(RangeU16 r, u16 v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_u32(RangeU32 r, u32 v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_u64(RangeU64 r, u64 v) { return v >= r.min && v <= r.max; }

static inline Bool range_contains_i8 (RangeI8  r, i8  v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_i16(RangeI16 r, i16 v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_i32(RangeI32 r, i32 v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_i64(RangeI64 r, i64 v) { return v >= r.min && v <= r.max; }

static inline Bool range_contains_f32(RangeF32 r, f32 v) { return v >= r.min && v <= r.max; }
static inline Bool range_contains_f64(RangeF64 r, f64 v) { return v >= r.min && v <= r.max; }

#endif /* KOLEKTO_RANGE */
