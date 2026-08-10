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
} range_u8;

typedef struct {
    u16 min;
    u16 max;
} range_u16;

typedef struct {
    u32 min;
    u32 max;
} range_u32;

typedef struct {
    u64 min;
    u64 max;
} range_u64;

// Signed
typedef struct {
    i8  min;
    i8  max;
} range_i8;

typedef struct {
    i16 min;
    i16 max;
} range_i16;

typedef struct {
    i32 min;
    i32 max;
} range_i32;

typedef struct {
    i64 min;
    i64 max;
} range_i64;

// Floating Point
typedef struct {
    f32 min;
    f32 max;
} range_f32;

typedef struct {
    f64 min;
    f64 max;
} range_f64;

#define NEW_RANGE(min, max)  {min, max}

// ────────────────────────────────────────────────────────────────────────────
// Public Functions
// ────────────────────────────────────────────────
#define range_is_empty(r)          ((r).min > (r).max)

#define range_contains(range, value) \
    _Generic((range),                \
        range_u8:  range_contains_u8,  \
        range_u16: range_contains_u16, \
        range_u32: range_contains_u32, \
        range_u64: range_contains_u64, \
        range_i8:  range_contains_i8,  \
        range_i16: range_contains_i16, \
        range_i32: range_contains_i32, \
        range_i64: range_contains_i64, \
        range_f32: range_contains_f32, \
        range_f64: range_contains_f64  \
    )((range), (value))

    
// ────────────────────────────────────────────────────────────────────────────
// Implementation
// ────────────────────────────────────────────────
static inline bool range_contains_u8 (range_u8  r, u8  v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_u16(range_u16 r, u16 v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_u32(range_u32 r, u32 v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_u64(range_u64 r, u64 v) { return v >= r.min && v <= r.max; }

static inline bool range_contains_i8 (range_i8  r, i8  v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_i16(range_i16 r, i16 v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_i32(range_i32 r, i32 v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_i64(range_i64 r, i64 v) { return v >= r.min && v <= r.max; }

static inline bool range_contains_f32(range_f32 r, f32 v) { return v >= r.min && v <= r.max; }
static inline bool range_contains_f64(range_f64 r, f64 v) { return v >= r.min && v <= r.max; }

#endif /* KOLEKTO_RANGE */
