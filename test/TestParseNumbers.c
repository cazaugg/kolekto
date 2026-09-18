#include "unity.h"
#include "../src/ParseNumbers.h"

#include <float.h>
#include <stdint.h>

void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_ParseNumber_U8_Valid(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "42"));
    TEST_ASSERT_EQUAL_UINT8(42, value);
}

static void Test_ParseNumber_U8_Boundaries(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "0"));
    TEST_ASSERT_EQUAL_UINT8(0, value);
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "255"));
    TEST_ASSERT_EQUAL_UINT8(255, value);
    TEST_ASSERT_FALSE(ParseNumber_U8(&value, "256"));
    TEST_ASSERT_FALSE(ParseNumber_U8(&value, "-1"));
}

static void Test_ParseNumber_U8_Hex(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "0xFF"));
    TEST_ASSERT_EQUAL_UINT8(255, value);
    TEST_ASSERT_TRUE(ParseHex_U8(&value, "ff"));
    TEST_ASSERT_EQUAL_UINT8(255, value);
    TEST_ASSERT_TRUE(ParseHex_U8(&value, "0x10"));
    TEST_ASSERT_EQUAL_UINT8(16, value);
}

static void Test_ParseNumber_U8_Decimal(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseDecimal_U8(&value, "42"));
    TEST_ASSERT_EQUAL_UINT8(42, value);
    // Lenient: base 10 parses the leading "0" of "0x10", ignores the rest.
    TEST_ASSERT_TRUE(ParseDecimal_U8(&value, "0x10"));
    TEST_ASSERT_EQUAL_UINT8(0, value);
    TEST_ASSERT_FALSE(ParseDecimal_U8(&value, "ff"));
}

static void Test_ParseNumber_U8_OctalAutodetect(void)
{
    // Base 0 autodetect: a leading 0 means octal.
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "010"));
    TEST_ASSERT_EQUAL_UINT8(8, value);
}

static void Test_ParseNumber_U8_Lenient(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "  12"));
    TEST_ASSERT_EQUAL_UINT8(12, value);
    TEST_ASSERT_TRUE(ParseNumber_U8(&value, "12x"));
    TEST_ASSERT_EQUAL_UINT8(12, value);
}

static void Test_ParseNumber_U8_InvalidUntouched(void)
{
    u8 value = 77;
    TEST_ASSERT_FALSE(ParseNumber_U8(&value, ""));
    TEST_ASSERT_FALSE(ParseNumber_U8(&value, "abc"));
    TEST_ASSERT_FALSE(ParseNumber_U8(&value, "99999999999999999999"));
    TEST_ASSERT_EQUAL_UINT8(77, value);
}

static void Test_ParseNumber_InRange_U8(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_InRange_U8(&value, "50", 10, 10, 100));
    TEST_ASSERT_EQUAL_UINT8(50, value);
    TEST_ASSERT_FALSE(ParseNumber_InRange_U8(&value, "5", 10, 10, 100));
    TEST_ASSERT_FALSE(ParseNumber_InRange_U8(&value, "101", 10, 10, 100));
    TEST_ASSERT_TRUE(ParseNumber_InRange_U8(&value, "ff", 16, 0, 255));
    TEST_ASSERT_EQUAL_UINT8(255, value);
    TEST_ASSERT_FALSE(ParseNumber_InRange_U8(&value, "10", 1, 0, 255));
}

static void Test_ParseNumber_U16_Boundaries(void)
{
    u16 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U16(&value, "65535"));
    TEST_ASSERT_EQUAL_UINT16(65535, value);
    TEST_ASSERT_FALSE(ParseNumber_U16(&value, "65536"));
    TEST_ASSERT_TRUE(ParseHex_U16(&value, "0xFFFF"));
    TEST_ASSERT_EQUAL_UINT16(65535, value);
}

static void Test_ParseNumber_U32_Boundaries(void)
{
    u32 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U32(&value, "4294967295"));
    TEST_ASSERT_EQUAL_UINT32(4294967295u, value);
    TEST_ASSERT_FALSE(ParseNumber_U32(&value, "4294967296"));
}

static void Test_ParseNumber_U64_Boundaries(void)
{
    u64 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_U64(&value, "18446744073709551615"));
    TEST_ASSERT_EQUAL_UINT64(18446744073709551615u, value);
    TEST_ASSERT_FALSE(ParseNumber_U64(&value, "18446744073709551616"));
}

static void Test_ParseNumber_I8_Negative(void)
{
    i8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_I8(&value, "-42"));
    TEST_ASSERT_EQUAL_INT8(-42, value);
    TEST_ASSERT_TRUE(ParseNumber_I8(&value, "-128"));
    TEST_ASSERT_EQUAL_INT8(-128, value);
    TEST_ASSERT_TRUE(ParseNumber_I8(&value, "127"));
    TEST_ASSERT_EQUAL_INT8(127, value);
    TEST_ASSERT_FALSE(ParseNumber_I8(&value, "128"));
    TEST_ASSERT_FALSE(ParseNumber_I8(&value, "-129"));
}

static void Test_ParseNumber_I16_Boundaries(void)
{
    i16 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_I16(&value, "-32768"));
    TEST_ASSERT_EQUAL_INT16(-32768, value);
    TEST_ASSERT_TRUE(ParseNumber_I16(&value, "32767"));
    TEST_ASSERT_EQUAL_INT16(32767, value);
    TEST_ASSERT_FALSE(ParseNumber_I16(&value, "32768"));
}

static void Test_ParseNumber_I32_Boundaries(void)
{
    i32 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_I32(&value, "-2147483648"));
    TEST_ASSERT_EQUAL_INT32(-2147483648, value);
    TEST_ASSERT_FALSE(ParseNumber_I32(&value, "2147483648"));
}

static void Test_ParseNumber_I64_Boundaries(void)
{
    i64 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_I64(&value, "9223372036854775807"));
    TEST_ASSERT_EQUAL_INT64(9223372036854775807, value);
    TEST_ASSERT_FALSE(ParseNumber_I64(&value, "9223372036854775808"));
    TEST_ASSERT_TRUE(ParseNumber_I64(&value, "-9223372036854775808"));
    TEST_ASSERT_EQUAL_INT64(INT64_MIN, value);
}

static void Test_ParseNumber_InRange_I32(void)
{
    i32 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_InRange_I32(&value, "-5", 10, -10, 10));
    TEST_ASSERT_EQUAL_INT32(-5, value);
    TEST_ASSERT_FALSE(ParseNumber_InRange_I32(&value, "-11", 10, -10, 10));
    TEST_ASSERT_FALSE(ParseNumber_InRange_I32(&value, "11", 10, -10, 10));
}

static void Test_ParseHex_I16_Negative(void)
{
    i16 value = 0;
    TEST_ASSERT_TRUE(ParseHex_I16(&value, "-ff"));
    TEST_ASSERT_EQUAL_INT16(-255, value);
}

static void Test_ParseNumber_F32_Valid(void)
{
    f32 value = 0.0f;
    TEST_ASSERT_TRUE(ParseNumber_F32(&value, "3.5"));
    TEST_ASSERT_EQUAL_FLOAT(3.5f, value);
    TEST_ASSERT_TRUE(ParseNumber_F32(&value, "-0.25"));
    TEST_ASSERT_EQUAL_FLOAT(-0.25f, value);
    TEST_ASSERT_TRUE(ParseNumber_F32(&value, "1e3"));
    TEST_ASSERT_EQUAL_FLOAT(1000.0f, value);
}

static void Test_ParseNumber_F32_Invalid(void)
{
    f32 value = 1.5f;
    TEST_ASSERT_FALSE(ParseNumber_F32(&value, ""));
    TEST_ASSERT_FALSE(ParseNumber_F32(&value, "abc"));
    TEST_ASSERT_FALSE(ParseNumber_F32(&value, "nan"));
    TEST_ASSERT_EQUAL_FLOAT(1.5f, value);
}

static void Test_ParseNumber_InRange_F32(void)
{
    f32 value = 0.0f;
    TEST_ASSERT_TRUE(ParseNumber_InRange_F32(&value, "5.5", 0, -10.0f, 10.0f));
    TEST_ASSERT_EQUAL_FLOAT(5.5f, value);
    TEST_ASSERT_FALSE(ParseNumber_InRange_F32(&value, "20.0", 0, -10.0f, 10.0f));
}

static void Test_ParseNumber_F64_Valid(void)
{
    f64 value = 0.0;
    TEST_ASSERT_TRUE(ParseNumber_F64(&value, "2.5"));
    TEST_ASSERT_EQUAL_UINT64(2500u, (u64)(value * 1000.0));
    TEST_ASSERT_TRUE(ParseNumber_F64(&value, "-1e-3"));
    TEST_ASSERT_TRUE((value < -0.0009) && (value > -0.0011));
    TEST_ASSERT_FALSE(ParseNumber_F64(&value, ""));
}

static void Test_ParseNumber_GenericMacro(void)
{
    u8 small = 0;
    u32 big = 0;
    i16 negative = 0;
    f64 precise = 0.0;

    TEST_ASSERT_TRUE(ParseNumber(small, "11"));
    TEST_ASSERT_EQUAL_UINT8(11, small);
    TEST_ASSERT_TRUE(ParseNumber(big, "100000"));
    TEST_ASSERT_EQUAL_UINT32(100000u, big);
    TEST_ASSERT_TRUE(ParseNumber(negative, "-7"));
    TEST_ASSERT_EQUAL_INT16(-7, negative);
    TEST_ASSERT_TRUE(ParseNumber(precise, "0.5"));
    TEST_ASSERT_EQUAL_UINT64(500u, (u64)(precise * 1000.0));
}

static void Test_ParseHex_GenericMacro(void)
{
    u16 value = 0;
    TEST_ASSERT_TRUE(ParseHex(value, "0x10"));
    TEST_ASSERT_EQUAL_UINT16(16, value);
}

static void Test_ParseDecimal_GenericMacro(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseDecimal(value, "42"));
    TEST_ASSERT_EQUAL_UINT8(42, value);
}

static void Test_ParseNumber_InRange_GenericMacro(void)
{
    u8 value = 0;
    TEST_ASSERT_TRUE(ParseNumber_InRange(value, "50", 10, 10, 100));
    TEST_ASSERT_EQUAL_UINT8(50, value);
    TEST_ASSERT_FALSE(ParseNumber_InRange(value, "5", 10, 10, 100));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_ParseNumber_U8_Valid);
    RUN_TEST(Test_ParseNumber_U8_Boundaries);
    RUN_TEST(Test_ParseNumber_U8_Hex);
    RUN_TEST(Test_ParseNumber_U8_Decimal);
    RUN_TEST(Test_ParseNumber_U8_OctalAutodetect);
    RUN_TEST(Test_ParseNumber_U8_Lenient);
    RUN_TEST(Test_ParseNumber_U8_InvalidUntouched);
    RUN_TEST(Test_ParseNumber_InRange_U8);
    RUN_TEST(Test_ParseNumber_U16_Boundaries);
    RUN_TEST(Test_ParseNumber_U32_Boundaries);
    RUN_TEST(Test_ParseNumber_U64_Boundaries);
    RUN_TEST(Test_ParseNumber_I8_Negative);
    RUN_TEST(Test_ParseNumber_I16_Boundaries);
    RUN_TEST(Test_ParseNumber_I32_Boundaries);
    RUN_TEST(Test_ParseNumber_I64_Boundaries);
    RUN_TEST(Test_ParseNumber_InRange_I32);
    RUN_TEST(Test_ParseHex_I16_Negative);
    RUN_TEST(Test_ParseNumber_F32_Valid);
    RUN_TEST(Test_ParseNumber_F32_Invalid);
    RUN_TEST(Test_ParseNumber_InRange_F32);
    RUN_TEST(Test_ParseNumber_F64_Valid);
    RUN_TEST(Test_ParseNumber_GenericMacro);
    RUN_TEST(Test_ParseHex_GenericMacro);
    RUN_TEST(Test_ParseDecimal_GenericMacro);
    RUN_TEST(Test_ParseNumber_InRange_GenericMacro);
    return UNITY_END();
}
