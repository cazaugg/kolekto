#include "parse_numbers.h"

#include <errno.h>
#include <float.h>
#include <stdlib.h>

// ── Shared engines ──────────────────────────────────────────────────────────

Bool ParseNumber_EngineUnsigned(u64 *num, String str, u8 base, u64 min, u64 max)
{
    ASSERT_OR(num && str) return false;
    if(base != 0u && (base < 2u || base > 36u)) return false;

    errno = 0;
    char *end = NULL;
    u64 value = strtoull(str, &end, (int)base);
    if(end == str) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow wraps, so always reject
    if(!(value >= min && value <= max)) return false;

    *num = value;
    return true;
}

Bool ParseNumber_EngineSigned(i64 *num, String str, u8 base, i64 min, i64 max)
{
    ASSERT_OR(num && str) return false;
    if(base != 0u && (base < 2u || base > 36u)) return false;

    errno = 0;
    char *end = NULL;
    i64 value = strtoll(str, &end, (int)base);
    if(end == str) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow saturates, so always reject
    if(!(value >= min && value <= max)) return false;

    *num = value;
    return true;
}

Bool ParseNumber_EngineF32(f32 *num, String str, f32 min, f32 max)
{
    ASSERT_OR(num && str) return false;

    errno = 0;
    char *end = NULL;
    f32 value = strtof(str, &end);
    if(end == str) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow yields HUGE_VALF, reject
    if(!(value >= min && value <= max)) return false; // also rejects NaN

    *num = value;
    return true;
}

Bool ParseNumber_EngineF64(f64 *num, String str, f64 min, f64 max)
{
    ASSERT_OR(num && str) return false;

    errno = 0;
    char *end = NULL;
    f64 value = strtod(str, &end);
    if(end == str) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow yields HUGE_VAL, reject
    if(!(value >= min && value <= max)) return false; // also rejects NaN

    *num = value;
    return true;
}

// ── Unsigned widths ─────────────────────────────────────────────────────────

Bool ParseNumber_U8(u8 *num, String str)
{
    return ParseNumber_InRange_U8(num, str, 0, 0, UINT8_MAX);
}

Bool ParseHex_U8(u8 *num, String str)
{
    return ParseNumber_InRange_U8(num, str, 16, 0, UINT8_MAX);
}

Bool ParseDecimal_U8(u8 *num, String str)
{
    return ParseNumber_InRange_U8(num, str, 10, 0, UINT8_MAX);
}

Bool ParseNumber_InRange_U8(u8 *num, String str, u8 base, u8 min, u8 max)
{
    ASSERT_OR(num) return false;
    u64 value = 0;
    if(!ParseNumber_EngineUnsigned(&value, str, base, (u64)min, (u64)max)) return false;
    *num = (u8)value;
    return true;
}

Bool ParseNumber_U16(u16 *num, String str)
{
    return ParseNumber_InRange_U16(num, str, 0, 0, UINT16_MAX);
}

Bool ParseHex_U16(u16 *num, String str)
{
    return ParseNumber_InRange_U16(num, str, 16, 0, UINT16_MAX);
}

Bool ParseDecimal_U16(u16 *num, String str)
{
    return ParseNumber_InRange_U16(num, str, 10, 0, UINT16_MAX);
}

Bool ParseNumber_InRange_U16(u16 *num, String str, u8 base, u16 min, u16 max)
{
    ASSERT_OR(num) return false;
    u64 value = 0;
    if(!ParseNumber_EngineUnsigned(&value, str, base, (u64)min, (u64)max)) return false;
    *num = (u16)value;
    return true;
}

Bool ParseNumber_U32(u32 *num, String str)
{
    return ParseNumber_InRange_U32(num, str, 0, 0, UINT32_MAX);
}

Bool ParseHex_U32(u32 *num, String str)
{
    return ParseNumber_InRange_U32(num, str, 16, 0, UINT32_MAX);
}

Bool ParseDecimal_U32(u32 *num, String str)
{
    return ParseNumber_InRange_U32(num, str, 10, 0, UINT32_MAX);
}

Bool ParseNumber_InRange_U32(u32 *num, String str, u8 base, u32 min, u32 max)
{
    ASSERT_OR(num) return false;
    u64 value = 0;
    if(!ParseNumber_EngineUnsigned(&value, str, base, (u64)min, (u64)max)) return false;
    *num = (u32)value;
    return true;
}

Bool ParseNumber_U64(u64 *num, String str)
{
    return ParseNumber_InRange_U64(num, str, 0, 0, UINT64_MAX);
}

Bool ParseHex_U64(u64 *num, String str)
{
    return ParseNumber_InRange_U64(num, str, 16, 0, UINT64_MAX);
}

Bool ParseDecimal_U64(u64 *num, String str)
{
    return ParseNumber_InRange_U64(num, str, 10, 0, UINT64_MAX);
}

Bool ParseNumber_InRange_U64(u64 *num, String str, u8 base, u64 min, u64 max)
{
    ASSERT_OR(num) return false;
    u64 value = 0;
    if(!ParseNumber_EngineUnsigned(&value, str, base, min, max)) return false;
    *num = value;
    return true;
}

// ── Signed widths ───────────────────────────────────────────────────────────

Bool ParseNumber_I8(i8 *num, String str)
{
    return ParseNumber_InRange_I8(num, str, 0, INT8_MIN, INT8_MAX);
}

Bool ParseHex_I8(i8 *num, String str)
{
    return ParseNumber_InRange_I8(num, str, 16, INT8_MIN, INT8_MAX);
}

Bool ParseDecimal_I8(i8 *num, String str)
{
    return ParseNumber_InRange_I8(num, str, 10, INT8_MIN, INT8_MAX);
}

Bool ParseNumber_InRange_I8(i8 *num, String str, u8 base, i8 min, i8 max)
{
    ASSERT_OR(num) return false;
    i64 value = 0;
    if(!ParseNumber_EngineSigned(&value, str, base, (i64)min, (i64)max)) return false;
    *num = (i8)value;
    return true;
}

Bool ParseNumber_I16(i16 *num, String str)
{
    return ParseNumber_InRange_I16(num, str, 0, INT16_MIN, INT16_MAX);
}

Bool ParseHex_I16(i16 *num, String str)
{
    return ParseNumber_InRange_I16(num, str, 16, INT16_MIN, INT16_MAX);
}

Bool ParseDecimal_I16(i16 *num, String str)
{
    return ParseNumber_InRange_I16(num, str, 10, INT16_MIN, INT16_MAX);
}

Bool ParseNumber_InRange_I16(i16 *num, String str, u8 base, i16 min, i16 max)
{
    ASSERT_OR(num) return false;
    i64 value = 0;
    if(!ParseNumber_EngineSigned(&value, str, base, (i64)min, (i64)max)) return false;
    *num = (i16)value;
    return true;
}

Bool ParseNumber_I32(i32 *num, String str)
{
    return ParseNumber_InRange_I32(num, str, 0, INT32_MIN, INT32_MAX);
}

Bool ParseHex_I32(i32 *num, String str)
{
    return ParseNumber_InRange_I32(num, str, 16, INT32_MIN, INT32_MAX);
}

Bool ParseDecimal_I32(i32 *num, String str)
{
    return ParseNumber_InRange_I32(num, str, 10, INT32_MIN, INT32_MAX);
}

Bool ParseNumber_InRange_I32(i32 *num, String str, u8 base, i32 min, i32 max)
{
    ASSERT_OR(num) return false;
    i64 value = 0;
    if(!ParseNumber_EngineSigned(&value, str, base, (i64)min, (i64)max)) return false;
    *num = (i32)value;
    return true;
}

Bool ParseNumber_I64(i64 *num, String str)
{
    return ParseNumber_InRange_I64(num, str, 0, INT64_MIN, INT64_MAX);
}

Bool ParseHex_I64(i64 *num, String str)
{
    return ParseNumber_InRange_I64(num, str, 16, INT64_MIN, INT64_MAX);
}

Bool ParseDecimal_I64(i64 *num, String str)
{
    return ParseNumber_InRange_I64(num, str, 10, INT64_MIN, INT64_MAX);
}

Bool ParseNumber_InRange_I64(i64 *num, String str, u8 base, i64 min, i64 max)
{
    ASSERT_OR(num) return false;
    i64 value = 0;
    if(!ParseNumber_EngineSigned(&value, str, base, min, max)) return false;
    *num = value;
    return true;
}

// ── Floating point (no base parameter in strtof/strtod: ParseHex falls ──────
// ── back to autodetect, the u8 base of the InRange variants is ignored) ─────

Bool ParseNumber_F32(f32 *num, String str)
{
    return ParseNumber_InRange_F32(num, str, 0, -FLT_MAX, FLT_MAX);
}

Bool ParseHex_F32(f32 *num, String str)
{
    return ParseNumber_F32(num, str);
}

Bool ParseDecimal_F32(f32 *num, String str)
{
    return ParseNumber_F32(num, str);
}

Bool ParseNumber_InRange_F32(f32 *num, String str, u8 base, f32 min, f32 max)
{
    ASSERT_OR(num) return false;
    UNUSED(base);
    f32 value = 0.0f;
    if(!ParseNumber_EngineF32(&value, str, min, max)) return false;
    *num = value;
    return true;
}

Bool ParseNumber_F64(f64 *num, String str)
{
    return ParseNumber_InRange_F64(num, str, 0, -DBL_MAX, DBL_MAX);
}

Bool ParseHex_F64(f64 *num, String str)
{
    return ParseNumber_F64(num, str);
}

Bool ParseDecimal_F64(f64 *num, String str)
{
    return ParseNumber_F64(num, str);
}

Bool ParseNumber_InRange_F64(f64 *num, String str, u8 base, f64 min, f64 max)
{
    ASSERT_OR(num) return false;
    UNUSED(base);
    f64 value = 0.0;
    if(!ParseNumber_EngineF64(&value, str, min, max)) return false;
    *num = value;
    return true;
}
