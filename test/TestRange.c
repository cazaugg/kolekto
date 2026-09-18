// ────────────────────────────────────────────────────────────────────────────
// TestRange — Unity tests for the Range module.
// ────────────────────────────────────────────────────────────────────────────

#include "unity.h"
#include "../src/Range.h"


void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_Range_IsEmpty_True(void)
{
    RangeU32 r = NEW_RANGE(10, 5);
    TEST_ASSERT_TRUE(Range_IsEmpty(r));
}

static void Test_Range_IsEmpty_False(void)
{
    RangeU32 r1 = NEW_RANGE(3, 7);
    TEST_ASSERT_FALSE(Range_IsEmpty(r1));

    RangeU32 r2 = NEW_RANGE(5, 5);
    TEST_ASSERT_FALSE(Range_IsEmpty(r2));
}

static void Test_Range_Contains_U8_True(void)
{
    RangeU8 r = NEW_RANGE(10, 20);

    // Middle
    TEST_ASSERT_TRUE(Range_Contains(r, 15));

    // Min
    TEST_ASSERT_TRUE(Range_Contains(r, 10));

    // Max
    TEST_ASSERT_TRUE(Range_Contains(r, 20));
}

static void Test_Range_Contains_U8_False(void)
{
    RangeU8 r = NEW_RANGE(10, 20);
    TEST_ASSERT_FALSE(Range_Contains(r, 9));

    TEST_ASSERT_FALSE(Range_Contains(r, 21));
}

static void Test_Range_Contains_I32_NegativeValues(void)
{
    RangeI32 r = NEW_RANGE(-10, 10);
    TEST_ASSERT_TRUE(Range_Contains(r, (i32)-5));
    TEST_ASSERT_FALSE(Range_Contains(r, (i32)-20));
    TEST_ASSERT_TRUE(Range_Contains(r, (i32)0));
}

static void Test_Range_Contains_F32_Precision(void)
{
    RangeF32 r = NEW_RANGE(0.5f, 1.5f);
    TEST_ASSERT_TRUE(Range_Contains(r, 1.0f));
    TEST_ASSERT_FALSE(Range_Contains(r, 0.0f));
    TEST_ASSERT_TRUE(Range_Contains(r, 0.5f));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_Range_IsEmpty_True);
    RUN_TEST(Test_Range_IsEmpty_False);
    RUN_TEST(Test_Range_Contains_U8_True);
    RUN_TEST(Test_Range_Contains_U8_False);
    RUN_TEST(Test_Range_Contains_I32_NegativeValues);
    RUN_TEST(Test_Range_Contains_F32_Precision);
    return UNITY_END();
}
