#include "ParseNumbers.h"

#include <errno.h>
#include <float.h>
#include <stdlib.h>

// ── Internal functions ───────────────────────────────────────────────────────

static Bool EngineUnsigned(u64 *number, String string, u8 base, u64 min, u64 max);
static Bool EngineSigned(i64 *number, String string, u8 base, i64 min, i64 max);
static Bool EngineF32(f32 *number, String string, f32 min, f32 max);
static Bool EngineF64(f64 *number, String string, f64 min, f64 max);

// ── Shared engines ──────────────────────────────────────────────────────────

static Bool EngineUnsigned(u64 *number, String string, u8 base, u64 min, u64 max)
{
    ASSERT_OR(number && string) return false;
    if(base != 0u && (base < 2u || base > 36u)) return false;

    errno = 0;
    char *end = NULL;
    u64 value = strtoull(string, &end, (int)base);
    if(end == string) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow wraps, so always reject
    if(!(value >= min && value <= max)) return false;

    *number = value;
    return true;
}

static Bool EngineSigned(i64 *number, String string, u8 base, i64 min, i64 max)
{
    ASSERT_OR(number && string) return false;
    if(base != 0u && (base < 2u || base > 36u)) return false;

    errno = 0;
    char *end = NULL;
    i64 value = strtoll(string, &end, (int)base);
    if(end == string) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow saturates, so always reject
    if(!(value >= min && value <= max)) return false;

    *number = value;
    return true;
}

static Bool EngineF32(f32 *number, String string, f32 min, f32 max)
{
    ASSERT_OR(number && string) return false;

    errno = 0;
    char *end = NULL;
    f32 value = strtof(string, &end);
    if(end == string) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow yields HUGE_VALF, reject
    if(!(value >= min && value <= max)) return false; // also rejects NaN

    *number = value;
    return true;
}

static Bool EngineF64(f64 *number, String string, f64 min, f64 max)
{
    ASSERT_OR(number && string) return false;

    errno = 0;
    char *end = NULL;
    f64 value = strtod(string, &end);
    if(end == string) return false;      // no digits consumed
    if(errno == ERANGE) return false; // overflow yields HUGE_VAL, reject
    if(!(value >= min && value <= max)) return false; // also rejects NaN

    *number = value;
    return true;
}

// ── Unsigned widths ─────────────────────────────────────────────────────────

Bool ParseNumber_U8(u8 *number, String string)
{
    return ParseNumber_InRange_U8(number, string, 0, 0, UINT8_MAX);
}

Bool ParseHex_U8(u8 *number, String string)
{
    return ParseNumber_InRange_U8(number, string, 16, 0, UINT8_MAX);
}

Bool ParseDecimal_U8(u8 *number, String string)
{
    return ParseNumber_InRange_U8(number, string, 10, 0, UINT8_MAX);
}

Bool ParseNumber_InRange_U8(u8 *number, String string, u8 base, u8 min, u8 max)
{
    ASSERT_OR(number) return false;
    u64 value = 0;
    if(!EngineUnsigned(&value, string, base, (u64)min, (u64)max)) return false;
    *number = (u8)value;
    return true;
}

Bool ParseNumber_U16(u16 *number, String string)
{
    return ParseNumber_InRange_U16(number, string, 0, 0, UINT16_MAX);
}

Bool ParseHex_U16(u16 *number, String string)
{
    return ParseNumber_InRange_U16(number, string, 16, 0, UINT16_MAX);
}

Bool ParseDecimal_U16(u16 *number, String string)
{
    return ParseNumber_InRange_U16(number, string, 10, 0, UINT16_MAX);
}

Bool ParseNumber_InRange_U16(u16 *number, String string, u8 base, u16 min, u16 max)
{
    ASSERT_OR(number) return false;
    u64 value = 0;
    if(!EngineUnsigned(&value, string, base, (u64)min, (u64)max)) return false;
    *number = (u16)value;
    return true;
}

Bool ParseNumber_U32(u32 *number, String string)
{
    return ParseNumber_InRange_U32(number, string, 0, 0, UINT32_MAX);
}

Bool ParseHex_U32(u32 *number, String string)
{
    return ParseNumber_InRange_U32(number, string, 16, 0, UINT32_MAX);
}

Bool ParseDecimal_U32(u32 *number, String string)
{
    return ParseNumber_InRange_U32(number, string, 10, 0, UINT32_MAX);
}

Bool ParseNumber_InRange_U32(u32 *number, String string, u8 base, u32 min, u32 max)
{
    ASSERT_OR(number) return false;
    u64 value = 0;
    if(!EngineUnsigned(&value, string, base, (u64)min, (u64)max)) return false;
    *number = (u32)value;
    return true;
}

Bool ParseNumber_U64(u64 *number, String string)
{
    return ParseNumber_InRange_U64(number, string, 0, 0, UINT64_MAX);
}

Bool ParseHex_U64(u64 *number, String string)
{
    return ParseNumber_InRange_U64(number, string, 16, 0, UINT64_MAX);
}

Bool ParseDecimal_U64(u64 *number, String string)
{
    return ParseNumber_InRange_U64(number, string, 10, 0, UINT64_MAX);
}

Bool ParseNumber_InRange_U64(u64 *number, String string, u8 base, u64 min, u64 max)
{
    ASSERT_OR(number) return false;
    u64 value = 0;
    if(!EngineUnsigned(&value, string, base, min, max)) return false;
    *number = value;
    return true;
}

// ── Signed widths ───────────────────────────────────────────────────────────

Bool ParseNumber_I8(i8 *number, String string)
{
    return ParseNumber_InRange_I8(number, string, 0, INT8_MIN, INT8_MAX);
}

Bool ParseHex_I8(i8 *number, String string)
{
    return ParseNumber_InRange_I8(number, string, 16, INT8_MIN, INT8_MAX);
}

Bool ParseDecimal_I8(i8 *number, String string)
{
    return ParseNumber_InRange_I8(number, string, 10, INT8_MIN, INT8_MAX);
}

Bool ParseNumber_InRange_I8(i8 *number, String string, u8 base, i8 min, i8 max)
{
    ASSERT_OR(number) return false;
    i64 value = 0;
    if(!EngineSigned(&value, string, base, (i64)min, (i64)max)) return false;
    *number = (i8)value;
    return true;
}

Bool ParseNumber_I16(i16 *number, String string)
{
    return ParseNumber_InRange_I16(number, string, 0, INT16_MIN, INT16_MAX);
}

Bool ParseHex_I16(i16 *number, String string)
{
    return ParseNumber_InRange_I16(number, string, 16, INT16_MIN, INT16_MAX);
}

Bool ParseDecimal_I16(i16 *number, String string)
{
    return ParseNumber_InRange_I16(number, string, 10, INT16_MIN, INT16_MAX);
}

Bool ParseNumber_InRange_I16(i16 *number, String string, u8 base, i16 min, i16 max)
{
    ASSERT_OR(number) return false;
    i64 value = 0;
    if(!EngineSigned(&value, string, base, (i64)min, (i64)max)) return false;
    *number = (i16)value;
    return true;
}

Bool ParseNumber_I32(i32 *number, String string)
{
    return ParseNumber_InRange_I32(number, string, 0, INT32_MIN, INT32_MAX);
}

Bool ParseHex_I32(i32 *number, String string)
{
    return ParseNumber_InRange_I32(number, string, 16, INT32_MIN, INT32_MAX);
}

Bool ParseDecimal_I32(i32 *number, String string)
{
    return ParseNumber_InRange_I32(number, string, 10, INT32_MIN, INT32_MAX);
}

Bool ParseNumber_InRange_I32(i32 *number, String string, u8 base, i32 min, i32 max)
{
    ASSERT_OR(number) return false;
    i64 value = 0;
    if(!EngineSigned(&value, string, base, (i64)min, (i64)max)) return false;
    *number = (i32)value;
    return true;
}

Bool ParseNumber_I64(i64 *number, String string)
{
    return ParseNumber_InRange_I64(number, string, 0, INT64_MIN, INT64_MAX);
}

Bool ParseHex_I64(i64 *number, String string)
{
    return ParseNumber_InRange_I64(number, string, 16, INT64_MIN, INT64_MAX);
}

Bool ParseDecimal_I64(i64 *number, String string)
{
    return ParseNumber_InRange_I64(number, string, 10, INT64_MIN, INT64_MAX);
}

Bool ParseNumber_InRange_I64(i64 *number, String string, u8 base, i64 min, i64 max)
{
    ASSERT_OR(number) return false;
    i64 value = 0;
    if(!EngineSigned(&value, string, base, min, max)) return false;
    *number = value;
    return true;
}

// ── Floating point (no base parameter in strtof/strtod: ParseHex falls ──────
// ── back to autodetect, the u8 base of the InRange variants is ignored) ─────

Bool ParseNumber_F32(f32 *number, String string)
{
    return ParseNumber_InRange_F32(number, string, 0, -FLT_MAX, FLT_MAX);
}

Bool ParseHex_F32(f32 *number, String string)
{
    return ParseNumber_F32(number, string);
}

Bool ParseDecimal_F32(f32 *number, String string)
{
    return ParseNumber_F32(number, string);
}

Bool ParseNumber_InRange_F32(f32 *number, String string, u8 base, f32 min, f32 max)
{
    ASSERT_OR(number) return false;
    UNUSED(base);
    f32 value = 0.0f;
    if(!EngineF32(&value, string, min, max)) return false;
    *number = value;
    return true;
}

Bool ParseNumber_F64(f64 *number, String string)
{
    return ParseNumber_InRange_F64(number, string, 0, -DBL_MAX, DBL_MAX);
}

Bool ParseHex_F64(f64 *number, String string)
{
    return ParseNumber_F64(number, string);
}

Bool ParseDecimal_F64(f64 *number, String string)
{
    return ParseNumber_F64(number, string);
}

Bool ParseNumber_InRange_F64(f64 *number, String string, u8 base, f64 min, f64 max)
{
    ASSERT_OR(number) return false;
    UNUSED(base);
    f64 value = 0.0;
    if(!EngineF64(&value, string, min, max)) return false;
    *number = value;
    return true;
}
