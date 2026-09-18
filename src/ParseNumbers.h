// ────────────────────────────────────────────────────────────────────────────
// ParseNumbers — type-safe parsing of text into numeric types.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// Parses text into any of the u8…u64 / i8…i64 / f32 / f64 types through the
// generic macros `ParseNumber`, `ParseHex`, `ParseDecimal` and
// `ParseNumber_InRange`. The destination is written only on success.
//
// Base handling (integers): base 0 autodetects — "0x…" is hex, a leading "0"
// is octal, anything else is decimal. `ParseHex` / `ParseDecimal` force the
// base. Parsing is lenient: leading whitespace is skipped and trailing garbage
// is ignored ("12x" parses as 12). Empty input, input without any digits and
// overflow are rejected. For floats the base argument is ignored.
//
// Usage:
//
//   u8 value = 0;
//   if(ParseNumber(value, "42")) { /* value == 42 */ }
//   if(ParseNumber_InRange(value, "ff", 16, 0, 200)) { /* ... */ }
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_PARSE_NUMBERS
#define KOLEKTO_PARSE_NUMBERS

#include "Datatypes.h"

/**
 * @brief Parse a number with an autodetected base into `number`.
 * @param number Destination lvalue (u8…u64, i8…i64, f32 or f64).
 * @param string Input text to parse.
 * @return true on success; `number` is left untouched on failure.
 */
#define ParseNumber(number, string) _Generic((number), \
    u8: ParseNumber_U8,     i8: ParseNumber_I8, \
    u16: ParseNumber_U16,   i16: ParseNumber_I16, \
    u32: ParseNumber_U32,   i32: ParseNumber_I32, \
    u64: ParseNumber_U64,   i64: ParseNumber_I64, \
    f32: ParseNumber_F32,   f64: ParseNumber_F64)(&(number), (string))

/**
 * @brief Parse a hexadecimal number into `number`.
 * @param number Destination lvalue (u8…u64, i8…i64, f32 or f64).
 * @param string Input text to parse.
 * @return true on success; `number` is left untouched on failure.
 */
#define ParseHex(number, string) _Generic((number), \
    u8: ParseHex_U8,     i8: ParseHex_I8, \
    u16: ParseHex_U16,   i16: ParseHex_I16, \
    u32: ParseHex_U32,   i32: ParseHex_I32, \
    u64: ParseHex_U64,   i64: ParseHex_I64, \
    f32: ParseNumber_F32, f64: ParseNumber_F64)(&(number), (string))

/**
 * @brief Parse a decimal number into `number`.
 * @param number Destination lvalue (u8…u64, i8…i64, f32 or f64).
 * @param string Input text to parse.
 * @return true on success; `number` is left untouched on failure.
 */
#define ParseDecimal(number, string) _Generic((number), \
    u8: ParseDecimal_U8,     i8: ParseDecimal_I8, \
    u16: ParseDecimal_U16,   i16: ParseDecimal_I16, \
    u32: ParseDecimal_U32,   i32: ParseDecimal_I32, \
    u64: ParseDecimal_U64,   i64: ParseDecimal_I64, \
    f32: ParseNumber_F32,    f64: ParseNumber_F64)(&(number), (string))

/**
 * @brief Parse a number in `base` and require it to lie within `[min, max]`.
 * @param number Destination lvalue (u8…u64, i8…i64, f32 or f64).
 * @param string Input text to parse.
 * @param base Numeric base (2…36, or 0 to autodetect; ignored for floats).
 * @param min Inclusive lower bound.
 * @param max Inclusive upper bound.
 * @return true on success; `number` is left untouched on failure.
 */
#define ParseNumber_InRange(number, string, base, min, max) _Generic((number), \
    u8: ParseNumber_InRange_U8,     i8: ParseNumber_InRange_I8, \
    u16: ParseNumber_InRange_U16,   i16: ParseNumber_InRange_I16, \
    u32: ParseNumber_InRange_U32,   i32: ParseNumber_InRange_I32, \
    u64: ParseNumber_InRange_U64,   i64: ParseNumber_InRange_I64, \
    f32: ParseNumber_InRange_F32,   f64: ParseNumber_InRange_F64)(&(number), (string), (base), (min), (max))

// ── Unsigned widths ─────────────────────────────────────────────────────────

/** @brief Parse an unsigned 8-bit value with an autodetected base. */
Bool ParseNumber_U8(u8 *number, String string);
/** @brief Parse an unsigned 8-bit hexadecimal value. */
Bool ParseHex_U8(u8 *number, String string);
/** @brief Parse an unsigned 8-bit decimal value. */
Bool ParseDecimal_U8(u8 *number, String string);
/** @brief Parse an unsigned 8-bit value within `[min, max]`. */
Bool ParseNumber_InRange_U8(u8 *number, String string, u8 base, u8 min, u8 max);

/** @brief Parse an unsigned 16-bit value with an autodetected base. */
Bool ParseNumber_U16(u16 *number, String string);
/** @brief Parse an unsigned 16-bit hexadecimal value. */
Bool ParseHex_U16(u16 *number, String string);
/** @brief Parse an unsigned 16-bit decimal value. */
Bool ParseDecimal_U16(u16 *number, String string);
/** @brief Parse an unsigned 16-bit value within `[min, max]`. */
Bool ParseNumber_InRange_U16(u16 *number, String string, u8 base, u16 min, u16 max);

/** @brief Parse an unsigned 32-bit value with an autodetected base. */
Bool ParseNumber_U32(u32 *number, String string);
/** @brief Parse an unsigned 32-bit hexadecimal value. */
Bool ParseHex_U32(u32 *number, String string);
/** @brief Parse an unsigned 32-bit decimal value. */
Bool ParseDecimal_U32(u32 *number, String string);
/** @brief Parse an unsigned 32-bit value within `[min, max]`. */
Bool ParseNumber_InRange_U32(u32 *number, String string, u8 base, u32 min, u32 max);

/** @brief Parse an unsigned 64-bit value with an autodetected base. */
Bool ParseNumber_U64(u64 *number, String string);
/** @brief Parse an unsigned 64-bit hexadecimal value. */
Bool ParseHex_U64(u64 *number, String string);
/** @brief Parse an unsigned 64-bit decimal value. */
Bool ParseDecimal_U64(u64 *number, String string);
/** @brief Parse an unsigned 64-bit value within `[min, max]`. */
Bool ParseNumber_InRange_U64(u64 *number, String string, u8 base, u64 min, u64 max);

// ── Signed widths ───────────────────────────────────────────────────────────

/** @brief Parse a signed 8-bit value with an autodetected base. */
Bool ParseNumber_I8(i8 *number, String string);
/** @brief Parse a signed 8-bit hexadecimal value. */
Bool ParseHex_I8(i8 *number, String string);
/** @brief Parse a signed 8-bit decimal value. */
Bool ParseDecimal_I8(i8 *number, String string);
/** @brief Parse a signed 8-bit value within `[min, max]`. */
Bool ParseNumber_InRange_I8(i8 *number, String string, u8 base, i8 min, i8 max);

/** @brief Parse a signed 16-bit value with an autodetected base. */
Bool ParseNumber_I16(i16 *number, String string);
/** @brief Parse a signed 16-bit hexadecimal value. */
Bool ParseHex_I16(i16 *number, String string);
/** @brief Parse a signed 16-bit decimal value. */
Bool ParseDecimal_I16(i16 *number, String string);
/** @brief Parse a signed 16-bit value within `[min, max]`. */
Bool ParseNumber_InRange_I16(i16 *number, String string, u8 base, i16 min, i16 max);

/** @brief Parse a signed 32-bit value with an autodetected base. */
Bool ParseNumber_I32(i32 *number, String string);
/** @brief Parse a signed 32-bit hexadecimal value. */
Bool ParseHex_I32(i32 *number, String string);
/** @brief Parse a signed 32-bit decimal value. */
Bool ParseDecimal_I32(i32 *number, String string);
/** @brief Parse a signed 32-bit value within `[min, max]`. */
Bool ParseNumber_InRange_I32(i32 *number, String string, u8 base, i32 min, i32 max);

/** @brief Parse a signed 64-bit value with an autodetected base. */
Bool ParseNumber_I64(i64 *number, String string);
/** @brief Parse a signed 64-bit hexadecimal value. */
Bool ParseHex_I64(i64 *number, String string);
/** @brief Parse a signed 64-bit decimal value. */
Bool ParseDecimal_I64(i64 *number, String string);
/** @brief Parse a signed 64-bit value within `[min, max]`. */
Bool ParseNumber_InRange_I64(i64 *number, String string, u8 base, i64 min, i64 max);

// ── Floating point ──────────────────────────────────────────────────────────

/** @brief Parse a 32-bit floating-point value. */
Bool ParseNumber_F32(f32 *number, String string);
/** @brief Parse a 32-bit floating-point value (base is ignored). */
Bool ParseHex_F32(f32 *number, String string);
/** @brief Parse a 32-bit floating-point value (base is ignored). */
Bool ParseDecimal_F32(f32 *number, String string);
/** @brief Parse a 32-bit floating-point value within `[min, max]`. */
Bool ParseNumber_InRange_F32(f32 *number, String string, u8 base, f32 min, f32 max);

/** @brief Parse a 64-bit floating-point value. */
Bool ParseNumber_F64(f64 *number, String string);
/** @brief Parse a 64-bit floating-point value (base is ignored). */
Bool ParseHex_F64(f64 *number, String string);
/** @brief Parse a 64-bit floating-point value (base is ignored). */
Bool ParseDecimal_F64(f64 *number, String string);
/** @brief Parse a 64-bit floating-point value within `[min, max]`. */
Bool ParseNumber_InRange_F64(f64 *number, String string, u8 base, f64 min, f64 max);

#endif /* KOLEKTO_PARSE_NUMBERS */
