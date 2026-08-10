#include "unity.h"
#include "../src/range.h"


void test_range_is_empty_true(void);
void test_range_is_empty_false(void);
void test_range_contains_u8_true(void);
void test_range_contains_u8_false(void);
void test_range_contains_i32_negative_values(void);
void test_range_contains_f32_precision(void);

void setUp(void);
void tearDown(void);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_range_is_empty_true(void)
{
    range_u32 r = NEW_RANGE(10, 5);
    TEST_ASSERT_TRUE(range_is_empty(r));
}

void test_range_is_empty_false(void)
{
    range_u32 r1 = NEW_RANGE(3, 7);
    TEST_ASSERT_FALSE(range_is_empty(r1));

    range_u32 r2 = NEW_RANGE(5, 5);
    TEST_ASSERT_FALSE(range_is_empty(r2));
}

void test_range_contains_u8_true(void)
{
    range_u8 r = NEW_RANGE(10, 20);
    
    // Middle
    TEST_ASSERT_TRUE(range_contains(r, 15));
    
    // Min
    TEST_ASSERT_TRUE(range_contains(r, 10));
    
    // Max
    TEST_ASSERT_TRUE(range_contains(r, 20));
}

void test_range_contains_u8_false(void)
{
    range_u8 r = NEW_RANGE(10, 20);
    TEST_ASSERT_FALSE(range_contains(r, 9));

    TEST_ASSERT_FALSE(range_contains(r, 21));
}


void test_range_contains_i32_negative_values(void)
{
    range_i32 r = NEW_RANGE(-10, 10);
    TEST_ASSERT_TRUE(range_contains(r, (i32)-5));
    TEST_ASSERT_FALSE(range_contains(r, (i32)-20));
    TEST_ASSERT_TRUE(range_contains(r, (i32)0));
}

void test_range_contains_f32_precision(void)
{
    range_f32 r = NEW_RANGE(0.5f, 1.5f);
    TEST_ASSERT_TRUE(range_contains(r, 1.0f));
    TEST_ASSERT_FALSE(range_contains(r, 0.0f));
    TEST_ASSERT_TRUE(range_contains(r, 0.5f));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_range_is_empty_true);
    RUN_TEST(test_range_is_empty_false);
    RUN_TEST(test_range_contains_u8_true);
    RUN_TEST(test_range_contains_u8_false);
    RUN_TEST(test_range_contains_i32_negative_values);
    RUN_TEST(test_range_contains_f32_precision);
    return UNITY_END();
}
