#include "unity.h"
#include "../src/range.h"


void Test_Range_IsEmptyTrue(void);
void Test_Range_IsEmptyFalse(void);
void Test_Range_IsSingleValueTrue(void);
void Test_Range_IsSingleValueFalse(void);
void Test_Range_ContainsU8True(void);
void Test_Range_ContainsU8False(void);
void Test_Range_ContainsU8_value_above_max(void);
void Test_Range_ContainsU8_value_at_min(void);
void Test_Range_ContainsU8_value_at_max(void);
void Test_Range_ContainsI32_negative_values(void);
void Test_Range_ContainsF32_precision(void);

void setUp(void);
void tearDown(void);

void setUp(void)
{
}

void tearDown(void)
{
}

void Test_Range_IsEmptyTrue(void)
{
    RangeU32 r = NEW_RANGE(10, 5);
    TEST_ASSERT_TRUE(Range_IsEmpty(r));
}

void Test_Range_IsEmptyFalse(void)
{
    RangeU32 r1 = NEW_RANGE(3, 7);
    TEST_ASSERT_FALSE(Range_IsEmpty(r1));

    RangeU32 r2 = NEW_RANGE(5, 5);
    TEST_ASSERT_FALSE(Range_IsEmpty(r2));
}

void Test_Range_ContainsU8True(void)
{
    RangeU8 r = NEW_RANGE(10, 20);
    
    // Middle
    TEST_ASSERT_TRUE(Range_Contains(r, 15));
    
    // Min
    TEST_ASSERT_TRUE(Range_Contains(r, 10));
    
    // Max
    TEST_ASSERT_TRUE(Range_Contains(r, 20));
}

void Test_Range_ContainsU8False(void)
{
    RangeU8 r = NEW_RANGE(10, 20);
    TEST_ASSERT_FALSE(Range_Contains(r, 9));

    TEST_ASSERT_FALSE(Range_Contains(r, 21));
}


void Test_Range_ContainsI32_negative_values(void)
{
    RangeI32 r = NEW_RANGE(-10, 10);
    TEST_ASSERT_TRUE(Range_Contains(r, (i32)-5));
    TEST_ASSERT_FALSE(Range_Contains(r, (i32)-20));
    TEST_ASSERT_TRUE(Range_Contains(r, (i32)0));
}

void Test_Range_ContainsF32_precision(void)
{
    RangeF32 r = NEW_RANGE(0.5f, 1.5f);
    TEST_ASSERT_TRUE(Range_Contains(r, 1.0f));
    TEST_ASSERT_FALSE(Range_Contains(r, 0.0f));
    TEST_ASSERT_TRUE(Range_Contains(r, 0.5f));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_Range_IsEmptyTrue);
    RUN_TEST(Test_Range_IsEmptyFalse);
    RUN_TEST(Test_Range_ContainsU8True);
    RUN_TEST(Test_Range_ContainsU8False);
    RUN_TEST(Test_Range_ContainsI32_negative_values);
    RUN_TEST(Test_Range_ContainsF32_precision);
    return UNITY_END();
}
