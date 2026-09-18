// ────────────────────────────────────────────────────────────────────────────
// Range — typed numeric ranges with generic containment checks.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// A range pairs a `min` and a `max` for one numeric type (`RangeU8` through
// `RangeF64`). Containment and emptiness are exposed as generic macros that
// dispatch on the range type, so the same call works for every width.
//
// Usage:
//
//   RangeU8 range = NEW_RANGE(10, 20);
//   if(Range_Contains(range, 15)) { /* ... */ }
//   if(Range_IsEmpty(range)) { /* ... */ }
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_RANGE
#define KOLEKTO_RANGE

#include "Datatypes.h"   

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

/** @brief Create a range value from a minimum and a maximum. */
#define NEW_RANGE(min, max)  {min, max}

// ────────────────────────────────────────────────────────────────────────────
// Public Functions
// ────────────────────────────────────────────────

/**
 * @brief Check whether a range is empty.
 * @param range Range to check.
 * @return true if `range.min > range.max`.
 */
#define Range_IsEmpty(range)          ((range).min > (range).max)

/**
 * @brief Check whether a value lies inside a range.
 * @param range Range to test against (RangeU8 … RangeF64).
 * @param value Value to test.
 * @return true if `value` is in `[range.min, range.max]`.
 */
#define Range_Contains(range, value) \
    _Generic((range),                \
        RangeU8:  RangeU8_Contains,  \
        RangeU16: RangeU16_Contains, \
        RangeU32: RangeU32_Contains, \
        RangeU64: RangeU64_Contains, \
        RangeI8:  RangeI8_Contains,  \
        RangeI16: RangeI16_Contains, \
        RangeI32: RangeI32_Contains, \
        RangeI64: RangeI64_Contains, \
        RangeF32: RangeF32_Contains, \
        RangeF64: RangeF64_Contains  \
    )((range), (value))


// ────────────────────────────────────────────────────────────────────────────
// Implementation
// ────────────────────────────────────────────────
static inline Bool RangeU8_Contains (RangeU8  range, u8  value) { return value >= range.min && value <= range.max; }
static inline Bool RangeU16_Contains(RangeU16 range, u16 value) { return value >= range.min && value <= range.max; }
static inline Bool RangeU32_Contains(RangeU32 range, u32 value) { return value >= range.min && value <= range.max; }
static inline Bool RangeU64_Contains(RangeU64 range, u64 value) { return value >= range.min && value <= range.max; }

static inline Bool RangeI8_Contains (RangeI8  range, i8  value) { return value >= range.min && value <= range.max; }
static inline Bool RangeI16_Contains(RangeI16 range, i16 value) { return value >= range.min && value <= range.max; }
static inline Bool RangeI32_Contains(RangeI32 range, i32 value) { return value >= range.min && value <= range.max; }
static inline Bool RangeI64_Contains(RangeI64 range, i64 value) { return value >= range.min && value <= range.max; }

static inline Bool RangeF32_Contains(RangeF32 range, f32 value) { return value >= range.min && value <= range.max; }
static inline Bool RangeF64_Contains(RangeF64 range, f64 value) { return value >= range.min && value <= range.max; }

#endif /* KOLEKTO_RANGE */
