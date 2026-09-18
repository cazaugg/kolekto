#include "datatypes.h"

#ifndef KOLEKTO_PARSE_NUMBERS
#define KOLEKTO_PARSE_NUMBERS

// ────────────────────────────────────────────────────────────────────────────
// Type-safe number parsing.
//
//   u8 value = 0;
//   if(ParseNumber(value, "42")) { /* value == 42 */ }
//
// The generic macros dispatch on the destination variable (which must be an
// lvalue of one of the u8…u64 / i8…i64 / f32 / f64 typedefs) and forward its
// address plus the remaining arguments to the matching typed function.
// The destination is written only on success; on failure it is untouched.
//
// Base handling (integers): base 0 autodetects — "0x…" is hex, a leading "0"
// is octal, anything else is decimal. ParseHex / ParseDecimal force the base.
// Parsing is lenient: leading whitespace is skipped and trailing garbage is
// ignored ("12x" parses as 12). Empty input, input without any digits, and
// overflow are rejected. For floats the base argument is accepted but
// ignored (strtof/strtod have no base parameter).
// ────────────────────────────────────────────────────────────────────────────

#define ParseNumber(num, str) _Generic((num), \
    u8: ParseNumber_U8,     i8: ParseNumber_I8, \
    u16: ParseNumber_U16,   i16: ParseNumber_I16, \
    u32: ParseNumber_U32,   i32: ParseNumber_I32, \
    u64: ParseNumber_U64,   i64: ParseNumber_I64, \
    f32: ParseNumber_F32,   f64: ParseNumber_F64)(&(num), (str))

#define ParseHex(num, str) _Generic((num), \
    u8: ParseHex_U8,     i8: ParseHex_I8, \
    u16: ParseHex_U16,   i16: ParseHex_I16, \
    u32: ParseHex_U32,   i32: ParseHex_I32, \
    u64: ParseHex_U64,   i64: ParseHex_I64, \
    f32: ParseNumber_F32, f64: ParseNumber_F64)(&(num), (str))

#define ParseDecimal(num, str) _Generic((num), \
    u8: ParseDecimal_U8,     i8: ParseDecimal_I8, \
    u16: ParseDecimal_U16,   i16: ParseDecimal_I16, \
    u32: ParseDecimal_U32,   i32: ParseDecimal_I32, \
    u64: ParseDecimal_U64,   i64: ParseDecimal_I64, \
    f32: ParseNumber_F32,    f64: ParseNumber_F64)(&(num), (str))

#define ParseNumber_InRange(num, str, base, min, max) _Generic((num), \
    u8: ParseNumber_InRange_U8,     i8: ParseNumber_InRange_I8, \
    u16: ParseNumber_InRange_U16,   i16: ParseNumber_InRange_I16, \
    u32: ParseNumber_InRange_U32,   i32: ParseNumber_InRange_I32, \
    u64: ParseNumber_InRange_U64,   i64: ParseNumber_InRange_I64, \
    f32: ParseNumber_InRange_F32,   f64: ParseNumber_InRange_F64)(&(num), (str), (base), (min), (max))

// ── Shared engines (one per value domain) ───────────────────────────────────

Bool ParseNumber_EngineUnsigned(u64 *num, String str, u8 base, u64 min, u64 max);
Bool ParseNumber_EngineSigned(i64 *num, String str, u8 base, i64 min, i64 max);
Bool ParseNumber_EngineF32(f32 *num, String str, f32 min, f32 max);
Bool ParseNumber_EngineF64(f64 *num, String str, f64 min, f64 max);

// ── Unsigned widths ─────────────────────────────────────────────────────────

Bool ParseNumber_U8(u8 *num, String str);
Bool ParseHex_U8(u8 *num, String str);
Bool ParseDecimal_U8(u8 *num, String str);
Bool ParseNumber_InRange_U8(u8 *num, String str, u8 base, u8 min, u8 max);

Bool ParseNumber_U16(u16 *num, String str);
Bool ParseHex_U16(u16 *num, String str);
Bool ParseDecimal_U16(u16 *num, String str);
Bool ParseNumber_InRange_U16(u16 *num, String str, u8 base, u16 min, u16 max);

Bool ParseNumber_U32(u32 *num, String str);
Bool ParseHex_U32(u32 *num, String str);
Bool ParseDecimal_U32(u32 *num, String str);
Bool ParseNumber_InRange_U32(u32 *num, String str, u8 base, u32 min, u32 max);

Bool ParseNumber_U64(u64 *num, String str);
Bool ParseHex_U64(u64 *num, String str);
Bool ParseDecimal_U64(u64 *num, String str);
Bool ParseNumber_InRange_U64(u64 *num, String str, u8 base, u64 min, u64 max);

// ── Signed widths ───────────────────────────────────────────────────────────

Bool ParseNumber_I8(i8 *num, String str);
Bool ParseHex_I8(i8 *num, String str);
Bool ParseDecimal_I8(i8 *num, String str);
Bool ParseNumber_InRange_I8(i8 *num, String str, u8 base, i8 min, i8 max);

Bool ParseNumber_I16(i16 *num, String str);
Bool ParseHex_I16(i16 *num, String str);
Bool ParseDecimal_I16(i16 *num, String str);
Bool ParseNumber_InRange_I16(i16 *num, String str, u8 base, i16 min, i16 max);

Bool ParseNumber_I32(i32 *num, String str);
Bool ParseHex_I32(i32 *num, String str);
Bool ParseDecimal_I32(i32 *num, String str);
Bool ParseNumber_InRange_I32(i32 *num, String str, u8 base, i32 min, i32 max);

Bool ParseNumber_I64(i64 *num, String str);
Bool ParseHex_I64(i64 *num, String str);
Bool ParseDecimal_I64(i64 *num, String str);
Bool ParseNumber_InRange_I64(i64 *num, String str, u8 base, i64 min, i64 max);

// ── Floating point ──────────────────────────────────────────────────────────

Bool ParseNumber_F32(f32 *num, String str);
Bool ParseHex_F32(f32 *num, String str);
Bool ParseDecimal_F32(f32 *num, String str);
Bool ParseNumber_InRange_F32(f32 *num, String str, u8 base, f32 min, f32 max);

Bool ParseNumber_F64(f64 *num, String str);
Bool ParseHex_F64(f64 *num, String str);
Bool ParseDecimal_F64(f64 *num, String str);
Bool ParseNumber_InRange_F64(f64 *num, String str, u8 base, f64 min, f64 max);

#endif /* KOLEKTO_PARSE_NUMBERS */
