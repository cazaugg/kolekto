#include "unity.h"
#include "../src/ascii.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_ASCII_IsValid_True(void)
{
    for (int c = 1; c < 128; ++c)
    {
        TEST_ASSERT_TRUE_MESSAGE(ASCII_IsValid((ascii)c), "All values 1-127 should be valid");
    }
}

static void Test_ASCII_IsValid_False(void)
{
    for (int c = 128; c < 256; ++c)
    {
        TEST_ASSERT_FALSE_MESSAGE(ASCII_IsValid((ascii)c), "All values 128-255 should be invalid");
    }
}

static void Test_ASCII_IsAlphaNumeric_letter(void)
{
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('A'));
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('Z'));
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('a'));
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('z'));
}

static void Test_ASCII_IsAlphaNumeric_digit(void)
{
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('0'));
    TEST_ASSERT_TRUE(ASCII_IsAlphaNumeric('9'));
}

static void Test_ASCII_IsAlphaNumeric_punctuation(void)
{
    TEST_ASSERT_FALSE(ASCII_IsAlphaNumeric(' '));
    TEST_ASSERT_FALSE(ASCII_IsAlphaNumeric('!'));
    TEST_ASSERT_FALSE(ASCII_IsAlphaNumeric('.'));
}

static void Test_ASCII_IsLetter_upper(void)
{
    TEST_ASSERT_TRUE(ASCII_IsLetter('A'));
    TEST_ASSERT_TRUE(ASCII_IsLetter('Z'));
}

static void Test_ASCII_IsLetter_lower(void)
{
    TEST_ASSERT_TRUE(ASCII_IsLetter('a'));
    TEST_ASSERT_TRUE(ASCII_IsLetter('z'));
}

static void Test_ASCII_IsLetter_digit(void)
{
    TEST_ASSERT_FALSE(ASCII_IsLetter('0'));
    TEST_ASSERT_FALSE(ASCII_IsLetter('9'));
}

static void Test_ASCII_IsLowerCase_true(void)
{
    TEST_ASSERT_TRUE(ASCII_IsLowerCase('a'));
    TEST_ASSERT_TRUE(ASCII_IsLowerCase('z'));
}

static void Test_ASCII_IsLowerCase_false(void)
{
    TEST_ASSERT_FALSE(ASCII_IsLowerCase('A'));
    TEST_ASSERT_FALSE(ASCII_IsLowerCase('Z'));
    TEST_ASSERT_FALSE(ASCII_IsLowerCase('0'));
}

static void Test_ASCII_IsUpperCase_true(void)
{
    TEST_ASSERT_TRUE(ASCII_IsUpperCase('A'));
    TEST_ASSERT_TRUE(ASCII_IsUpperCase('Z'));
}

static void Test_ASCII_IsUpperCase_false(void)
{
    TEST_ASSERT_FALSE(ASCII_IsUpperCase('a'));
    TEST_ASSERT_FALSE(ASCII_IsUpperCase('z'));
    TEST_ASSERT_FALSE(ASCII_IsUpperCase('0'));
}

static void Test_ASCII_IsDigit_true(void)
{
    TEST_ASSERT_TRUE(ASCII_IsDigit('0'));
    TEST_ASSERT_TRUE(ASCII_IsDigit('9'));
}

static void Test_ASCII_IsDigit_false(void)
{
    TEST_ASSERT_FALSE(ASCII_IsDigit('A'));
    TEST_ASSERT_FALSE(ASCII_IsDigit('a'));
    TEST_ASSERT_FALSE(ASCII_IsDigit(' '));
}

static void Test_ASCII_IsHexDigit_upper(void)
{
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('A'));
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('F'));
}

static void Test_ASCII_IsHexDigit_lower(void)
{
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('a'));
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('f'));
}

static void Test_ASCII_IsHexDigit_nonhex_letter(void)
{
    TEST_ASSERT_FALSE(ASCII_IsHexDigit('G'));
    TEST_ASSERT_FALSE(ASCII_IsHexDigit('Z'));
    TEST_ASSERT_FALSE(ASCII_IsHexDigit('g'));
    TEST_ASSERT_FALSE(ASCII_IsHexDigit('z'));
}

static void Test_ASCII_IsHexDigit_digit(void)
{
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('0'));
    TEST_ASSERT_TRUE(ASCII_IsHexDigit('9'));
}

static void Test_ASCII_IsPrintable_letter(void)
{
    TEST_ASSERT_TRUE(ASCII_IsPrintable('A'));
    TEST_ASSERT_TRUE(ASCII_IsPrintable('z'));
}

static void Test_ASCII_IsPrintable_digit(void)
{
    TEST_ASSERT_TRUE(ASCII_IsPrintable('0'));
}

static void Test_ASCII_IsPrintable_punctuation(void)
{
    TEST_ASSERT_TRUE(ASCII_IsPrintable('!'));
    TEST_ASSERT_TRUE(ASCII_IsPrintable('.'));
}

static void Test_ASCII_IsPrintable_space(void)
{
    TEST_ASSERT_TRUE(ASCII_IsPrintable(' '));
}

static void Test_ASCII_IsPrintable_control(void)
{
    TEST_ASSERT_FALSE(ASCII_IsPrintable('\t'));
    TEST_ASSERT_FALSE(ASCII_IsPrintable('\n'));
    TEST_ASSERT_FALSE(ASCII_IsPrintable('\0'));
}

static void Test_ASCII_IsSpace_space(void)
{
    TEST_ASSERT_TRUE(ASCII_IsSpace(' '));
}

static void Test_ASCII_IsSpace_tab(void)
{
    TEST_ASSERT_TRUE(ASCII_IsSpace('\t'));
}

static void Test_ASCII_IsSpace_newline(void)
{
    TEST_ASSERT_TRUE(ASCII_IsSpace('\n'));
}

static void Test_ASCII_IsSpace_letter(void)
{
    TEST_ASSERT_FALSE(ASCII_IsSpace('A'));
}

static void Test_ASCII_IsPunctuation_true(void)
{
    TEST_ASSERT_TRUE(ASCII_IsPunctuation('!'));
    TEST_ASSERT_TRUE(ASCII_IsPunctuation('.'));
    TEST_ASSERT_TRUE(ASCII_IsPunctuation('?'));
    TEST_ASSERT_TRUE(ASCII_IsPunctuation(','));
}

static void Test_ASCII_IsPunctuation_false(void)
{
    TEST_ASSERT_FALSE(ASCII_IsPunctuation('A'));
    TEST_ASSERT_FALSE(ASCII_IsPunctuation('a'));
    TEST_ASSERT_FALSE(ASCII_IsPunctuation('0'));
    TEST_ASSERT_FALSE(ASCII_IsPunctuation(' '));
}

static void Test_ASCII_IsControlCode_true(void)
{
    for (int c = 0; c < 32; ++c)
    {
        TEST_ASSERT_TRUE_MESSAGE(ASCII_IsControlCode((ascii)c), "Values 0-31 should be control codes");
    }
    TEST_ASSERT_TRUE(ASCII_IsControlCode(127));
}

static void Test_ASCII_IsControlCode_false(void)
{
    TEST_ASSERT_FALSE(ASCII_IsControlCode(' '));
    TEST_ASSERT_FALSE(ASCII_IsControlCode('A'));
    TEST_ASSERT_FALSE(ASCII_IsControlCode('0'));
}

static void Test_ASCII_ToLower_upper(void)
{
    TEST_ASSERT_EQUAL('a', ASCII_ToLower('A'));
    TEST_ASSERT_EQUAL('z', ASCII_ToLower('Z'));
}

static void Test_ASCII_ToLower_lower(void)
{
    TEST_ASSERT_EQUAL('a', ASCII_ToLower('a'));
    TEST_ASSERT_EQUAL('z', ASCII_ToLower('z'));
}

static void Test_ASCII_ToLower_nonletter(void)
{
    TEST_ASSERT_EQUAL('0', ASCII_ToLower('0'));
    TEST_ASSERT_EQUAL(' ', ASCII_ToLower(' '));
}

static void Test_ASCII_ToUpper_lower(void)
{
    TEST_ASSERT_EQUAL('A', ASCII_ToUpper('a'));
    TEST_ASSERT_EQUAL('Z', ASCII_ToUpper('z'));
}

static void Test_ASCII_ToUpper_upper(void)
{
    TEST_ASSERT_EQUAL('A', ASCII_ToUpper('A'));
    TEST_ASSERT_EQUAL('Z', ASCII_ToUpper('Z'));
}

static void Test_ASCII_ToUpper_nonletter(void)
{
    TEST_ASSERT_EQUAL('0', ASCII_ToUpper('0'));
    TEST_ASSERT_EQUAL(' ', ASCII_ToUpper(' '));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_ASCII_IsValid_True);
    RUN_TEST(Test_ASCII_IsValid_False);
    RUN_TEST(Test_ASCII_IsAlphaNumeric_letter);
    RUN_TEST(Test_ASCII_IsAlphaNumeric_digit);
    RUN_TEST(Test_ASCII_IsAlphaNumeric_punctuation);
    RUN_TEST(Test_ASCII_IsLetter_upper);
    RUN_TEST(Test_ASCII_IsLetter_lower);
    RUN_TEST(Test_ASCII_IsLetter_digit);
    RUN_TEST(Test_ASCII_IsLowerCase_true);
    RUN_TEST(Test_ASCII_IsLowerCase_false);
    RUN_TEST(Test_ASCII_IsUpperCase_true);
    RUN_TEST(Test_ASCII_IsUpperCase_false);
    RUN_TEST(Test_ASCII_IsDigit_true);
    RUN_TEST(Test_ASCII_IsDigit_false);
    RUN_TEST(Test_ASCII_IsHexDigit_upper);
    RUN_TEST(Test_ASCII_IsHexDigit_lower);
    RUN_TEST(Test_ASCII_IsHexDigit_nonhex_letter);
    RUN_TEST(Test_ASCII_IsHexDigit_digit);
    RUN_TEST(Test_ASCII_IsPrintable_letter);
    RUN_TEST(Test_ASCII_IsPrintable_digit);
    RUN_TEST(Test_ASCII_IsPrintable_punctuation);
    RUN_TEST(Test_ASCII_IsPrintable_space);
    RUN_TEST(Test_ASCII_IsPrintable_control);
    RUN_TEST(Test_ASCII_IsSpace_space);
    RUN_TEST(Test_ASCII_IsSpace_tab);
    RUN_TEST(Test_ASCII_IsSpace_newline);
    RUN_TEST(Test_ASCII_IsSpace_letter);
    RUN_TEST(Test_ASCII_IsPunctuation_true);
    RUN_TEST(Test_ASCII_IsPunctuation_false);
    RUN_TEST(Test_ASCII_IsControlCode_true);
    RUN_TEST(Test_ASCII_IsControlCode_false);
    RUN_TEST(Test_ASCII_ToLower_upper);
    RUN_TEST(Test_ASCII_ToLower_lower);
    RUN_TEST(Test_ASCII_ToLower_nonletter);
    RUN_TEST(Test_ASCII_ToUpper_lower);
    RUN_TEST(Test_ASCII_ToUpper_upper);
    RUN_TEST(Test_ASCII_ToUpper_nonletter);
    return UNITY_END();
}
