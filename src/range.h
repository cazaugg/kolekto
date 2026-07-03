#ifndef KOLEKTO_RANGE
#define KOLEKTO_RANGE

#include "datatypes.h"   

// Ranges allow to check if a value is within certain boundries
// Ther is a specific range type for every basic number type, 
// however the functions and checks are generic.

// ────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────

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

// ────────────────────────────────────────────────
// Public Functions
// ────────────────────────────────────────────────
#define Range_IsEmpty(r)          ((r).min > (r).max)

#define Range_Contains(range, value) \
    _Generic((range),                \
        RangeU8:  Range_ContainsU8,  \
        RangeU16: Range_ContainsU16, \
        RangeU32: Range_ContainsU32, \
        RangeU64: Range_ContainsU64, \
        RangeI8:  Range_ContainsI8,  \
        RangeI16: Range_ContainsI16, \
        RangeI32: Range_ContainsI32, \
        RangeI64: Range_ContainsI64, \
        RangeF32: Range_ContainsF32, \
        RangeF64: Range_ContainsF64  \
    )((range), (value))

    
// ────────────────────────────────────────────────
// Implementation
// ────────────────────────────────────────────────
static inline bool Range_ContainsU8 (RangeU8  r, u8  v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsU16(RangeU16 r, u16 v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsU32(RangeU32 r, u32 v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsU64(RangeU64 r, u64 v) { return v >= r.min && v <= r.max; }

static inline bool Range_ContainsI8 (RangeI8  r, i8  v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsI16(RangeI16 r, i16 v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsI32(RangeI32 r, i32 v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsI64(RangeI64 r, i64 v) { return v >= r.min && v <= r.max; }

static inline bool Range_ContainsF32(RangeF32 r, f32 v) { return v >= r.min && v <= r.max; }
static inline bool Range_ContainsF64(RangeF64 r, f64 v) { return v >= r.min && v <= r.max; }

#endif /* KOLEKTO_RANGE */
