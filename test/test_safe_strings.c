#include "unity.h"
#include "../src/safe_string.h"

void test_string_literal_length(void);
void test_string_builder_length(void);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_string_literal_length(void)
{
    const char *empty = "";
    const char *word = "abcd";
    const char *phrase = "hello world";
    TEST_ASSERT_EQUAL_UINT32(0, string_length(empty));
    TEST_ASSERT_EQUAL_UINT32(4, string_length(word));
    TEST_ASSERT_EQUAL_UINT32(11, string_length(phrase));
}

void test_string_builder_length(void)
{
    char buffer[32] = {0};
    string_builder sb = {.data = buffer, .capacity = 32, .length = 0};
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));

    string_set(&sb, "hello");
    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));

    string_append(&sb, " world");
    TEST_ASSERT_EQUAL_UINT32(11, string_length(sb));

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_string_literal_length);
    RUN_TEST(test_string_builder_length);
    return UNITY_END();
}
