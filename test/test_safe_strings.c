#include "unity.h"
#include "../src/safe_string.h"

void test_string_literal_length(void);
void test_string_builder_length(void);
void test_string_builder_capacity(void);
void test_string_literal_empty(void);
void test_string_builder_empty(void);
void test_string_equal_true(void);
void test_string_equal_false(void);
void test_string_equal_builder(void);
void test_string_compare_equal(void);
void test_string_compare_less(void);
void test_string_compare_greater(void);
void test_string_compare_null(void);
void test_string_starts_with_true(void);
void test_string_starts_with_false(void);
void test_string_ends_with_true(void);
void test_string_ends_with_false(void);
void test_string_contains_true(void);
void test_string_contains_false(void);
void test_string_find(void);
void test_string_count(void);
void test_string_set(void);
void test_string_clear(void);
void test_string_append(void);
void test_string_append_char(void);
void test_string_trim_start(void);
void test_string_trim_end(void);
void test_string_trim(void);
void test_string_to_upper(void);
void test_string_to_lower(void);
void test_string_join(void);
void test_string_reverse(void);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_string_literal_length(void)
{
    string empty = "";
    string word = "abcd";
    string phrase = "hello world";
    TEST_ASSERT_EQUAL_UINT32(0, string_length(empty));
    TEST_ASSERT_EQUAL_UINT32(4, string_length(word));
    TEST_ASSERT_EQUAL_UINT32(11, string_length(phrase));
}

void test_string_builder_length(void)
{
    string_builder sb = NEW_STRING_BUILDER(32, "");
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));

    string_set(&sb, "hello");
    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));

    string_append(&sb, " world");
    TEST_ASSERT_EQUAL_UINT32(11, string_length(sb));

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));
}

void test_string_builder_capacity(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "abc");

    TEST_ASSERT_EQUAL_UINT32(13, string_builder_capacity(sb));

    string_append(&sb, "123456789");
    TEST_ASSERT_EQUAL_UINT32(4, string_builder_capacity(sb));

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(16, string_builder_capacity(sb));
}

void test_string_literal_empty(void)
{
    string empty = "";
    string single = "a";
    string word = "hello";
    TEST_ASSERT_TRUE(string_empty(empty));
    TEST_ASSERT_FALSE(string_empty(single));
    TEST_ASSERT_FALSE(string_empty(word));
}

void test_string_builder_empty(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "");
    TEST_ASSERT_TRUE(string_empty(sb));

    string_set(&sb, "x");
    TEST_ASSERT_FALSE(string_empty(sb));

    string_clear(&sb);
    TEST_ASSERT_TRUE(string_empty(sb));
}

void test_string_equal_true(void)
{
    string empty = "";
    string word = "abcd";
    string_builder phrase = NEW_STRING_BUILDER(20, "hello world");
    TEST_ASSERT_TRUE(string_equal(empty, ""));
    TEST_ASSERT_TRUE(string_equal(word, "abcd"));
    TEST_ASSERT_TRUE(string_equal(phrase, "hello world"));
}

void test_string_equal_false(void)
{
    string abc = "abc";
    string_builder phrase = NEW_STRING_BUILDER(20, "abd");
    string empty = "";
    TEST_ASSERT_FALSE(string_equal(abc, "abd"));
    TEST_ASSERT_FALSE(string_equal(abc, ""));
    TEST_ASSERT_FALSE(string_equal(empty, "abc"));
    TEST_ASSERT_FALSE(string_equal(phrase, "abc"));
}

void test_string_equal_builder(void)
{
    char buffer[16] = {0};
    string_builder sb = {.data = buffer, .capacity = 16, .length = 0};
    string hello = "hello";
    string world = "world";

    string_set(&sb, hello);
    TEST_ASSERT_TRUE(string_equal(sb, hello));
    TEST_ASSERT_TRUE(string_equal(hello, sb));
    TEST_ASSERT_FALSE(string_equal(sb, world));

    char buffer2[16] = {0};
    string_builder sb2 = {.data = buffer2, .capacity = 16, .length = 0};
    string_set(&sb2, hello);
    TEST_ASSERT_TRUE(string_equal(sb, sb2));
}

void test_string_compare_equal(void)
{
    string a = "hello";
    string b = "hello";
    string empty = "";
    string_builder c = NEW_STRING_BUILDER(20, "hello");
    TEST_ASSERT_EQUAL_INT8(0, string_compare(a, b));
    TEST_ASSERT_EQUAL_INT8(0, string_compare(a, c));
    TEST_ASSERT_EQUAL_INT8(0, string_compare(empty, empty));
}

void test_string_compare_less(void)
{
    string a = "abc";
    string b = "abd";
    string_builder c = NEW_STRING_BUILDER(20, "hello");
    string prefix = "ab";
    TEST_ASSERT_TRUE(string_compare(a, b) < 0);
    TEST_ASSERT_TRUE(string_compare(c, b) > 0);
    TEST_ASSERT_TRUE(string_compare(prefix, a) < 0);
}

void test_string_compare_greater(void)
{
    string a = "abd";
    string b = "abc";
    string_builder c = NEW_STRING_BUILDER(20, "hello");
    string longer = "abcd";
    TEST_ASSERT_TRUE(string_compare(a, b) > 0);
    TEST_ASSERT_TRUE(string_compare(a, c) < 0);
    TEST_ASSERT_TRUE(string_compare(longer, "abc") > 0);
}

void test_string_compare_null(void)
{
    TEST_ASSERT_EQUAL_INT8(-1, string_literal_compare(NULL, "a"));
    TEST_ASSERT_EQUAL_INT8(1, string_literal_compare("a", NULL));
    TEST_ASSERT_EQUAL_INT8(-1, string_literal_compare(NULL, NULL));
}

void test_string_starts_with_true(void)
{
    string sentence = "hello world";
    TEST_ASSERT_TRUE(string_starts_with(sentence, "hello"));
    TEST_ASSERT_TRUE(string_starts_with(sentence, "h"));
    TEST_ASSERT_TRUE(string_starts_with(sentence, "hello world"));
    TEST_ASSERT_TRUE(string_starts_with(sentence, ""));

    string_builder sentence2 = NEW_STRING_BUILDER(11, "hello world");
    TEST_ASSERT_TRUE(string_starts_with(sentence2, "hello"));
    TEST_ASSERT_TRUE(string_starts_with(sentence2, "h"));
    TEST_ASSERT_TRUE(string_starts_with(sentence2, "hello world"));
    TEST_ASSERT_TRUE(string_starts_with(sentence2, ""));
}

void test_string_starts_with_false(void)
{
    string sentence = "hello world";
    TEST_ASSERT_FALSE(string_starts_with(sentence, "world"));
    TEST_ASSERT_FALSE(string_starts_with(sentence, "HELLO"));
    TEST_ASSERT_FALSE(string_starts_with(sentence, "hello world!"));

    string_builder sentence2 = NEW_STRING_BUILDER(11, "hello world");
    TEST_ASSERT_FALSE(string_starts_with(sentence2, "world"));
    TEST_ASSERT_FALSE(string_starts_with(sentence2, "HELLO"));
    TEST_ASSERT_FALSE(string_starts_with(sentence2, "hello world!"));
}

void test_string_ends_with_true(void)
{
    string sentence = "hello world";
    TEST_ASSERT_TRUE(string_ends_with(sentence, "world"));
    TEST_ASSERT_TRUE(string_ends_with(sentence, "d"));
    TEST_ASSERT_TRUE(string_ends_with(sentence, "hello world"));
    TEST_ASSERT_TRUE(string_ends_with(sentence, ""));

    string_builder sentence2 = NEW_STRING_BUILDER(11, "hello world");
    TEST_ASSERT_TRUE(string_ends_with(sentence2, "world"));
    TEST_ASSERT_TRUE(string_ends_with(sentence2, "d"));
    TEST_ASSERT_TRUE(string_ends_with(sentence2, "hello world"));
    TEST_ASSERT_TRUE(string_ends_with(sentence2, ""));
}

void test_string_ends_with_false(void)
{
    string sentence = "hello world";
    TEST_ASSERT_FALSE(string_ends_with(sentence, "hello"));
    TEST_ASSERT_FALSE(string_ends_with(sentence, "WORLD"));
    TEST_ASSERT_FALSE(string_ends_with(sentence, "xhello world"));
    TEST_ASSERT_FALSE(string_ends_with(sentence, "hello world!"));
    
    string_builder sentence2 = NEW_STRING_BUILDER(11, "hello world");
    TEST_ASSERT_FALSE(string_ends_with(sentence2, "hello"));
    TEST_ASSERT_FALSE(string_ends_with(sentence2, "WORLD"));
    TEST_ASSERT_FALSE(string_ends_with(sentence2, "xhello world"));
    TEST_ASSERT_FALSE(string_ends_with(sentence2, "hello world!"));
}

void test_string_contains_true(void)
{
    string sentence = "hello world";
    TEST_ASSERT_TRUE(string_contains(sentence, "world"));
    TEST_ASSERT_TRUE(string_contains(sentence, "hello"));
    TEST_ASSERT_TRUE(string_contains(sentence, "lo wo"));
    TEST_ASSERT_TRUE(string_contains(sentence, " "));
}

void test_string_contains_false(void)
{
    string sentence = "hello world";
    TEST_ASSERT_FALSE(string_contains(sentence, "xyz"));
    TEST_ASSERT_FALSE(string_contains(sentence, "WORLD"));
    TEST_ASSERT_FALSE(string_contains(sentence, "hello world!"));
}

void test_string_find(void)
{
    string sentence = "hello world";
    TEST_ASSERT_EQUAL_UINT32(0, string_find(sentence, "hello"));
    TEST_ASSERT_EQUAL_UINT32(6, string_find(sentence, "world"));
    TEST_ASSERT_EQUAL_UINT32(4, string_find(sentence, "o"));
    TEST_ASSERT_EQUAL_UINT32(4, string_find(sentence, "o w"));
    TEST_ASSERT_EQUAL_UINT32(0, string_find(sentence, "hello world"));
}

void test_string_count(void)
{
    string sentence = "hello world";
    TEST_ASSERT_EQUAL_UINT32(3, string_count(sentence, "l"));
    TEST_ASSERT_EQUAL_UINT32(2, string_count(sentence, "o"));
    TEST_ASSERT_EQUAL_UINT32(1, string_count(sentence, "world"));
    TEST_ASSERT_EQUAL_UINT32(0, string_count("abc", "z"));
    TEST_ASSERT_EQUAL_UINT32(1, string_count("aaa", "aaa"));
    TEST_ASSERT_EQUAL_UINT32(2, string_count("aba", "a"));
}

void test_string_set(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "hello");

    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    string_set(&sb, "hi");
    TEST_ASSERT_EQUAL_UINT32(2, string_length(sb));
    TEST_ASSERT_EQUAL_STRING("hi", sb.data);

    string_set(&sb, "");
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_clear(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "hello");
    TEST_ASSERT_FALSE(string_empty(sb));

    string_clear(&sb);
    TEST_ASSERT_TRUE(string_empty(sb));
    TEST_ASSERT_EQUAL_UINT32(0, string_length(sb));
}

void test_string_append(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "hello");

    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));

    TEST_ASSERT_TRUE(string_append(&sb, " world"));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, string_length(sb));

    string_clear(&sb);
    TEST_ASSERT_TRUE(string_append(&sb, "abc"));
    TEST_ASSERT_EQUAL_STRING("abc", sb.data);
}

void test_string_append_char(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "abc ");

    TEST_ASSERT_TRUE(string_append_char(&sb, 'h'));
    TEST_ASSERT_TRUE(string_append_char(&sb, 'i'));
    TEST_ASSERT_EQUAL_STRING("abc hi", sb.data);
    TEST_ASSERT_EQUAL_UINT32(6, string_length(sb));

    string_append_char(&sb, '!');
    TEST_ASSERT_EQUAL_STRING("abc hi!", sb.data);
}

void test_string_trim_start(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "   hello");

    TEST_ASSERT_EQUAL_UINT32(3, string_trim_start(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, string_trim_start(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    string_set(&sb, "\t\n \r\v\f x");
    TEST_ASSERT_EQUAL_UINT32(7, string_trim_start(&sb));
    TEST_ASSERT_EQUAL_STRING("x", sb.data);

    string_set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, string_trim_start(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
    TEST_ASSERT_TRUE(string_empty(sb));

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_trim_start(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_trim_end(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "hello   ");

    TEST_ASSERT_EQUAL_UINT32(3, string_trim_end(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, string_length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, string_trim_end(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    string_set(&sb, "x \t\n \r\v\f");
    TEST_ASSERT_EQUAL_UINT32(7, string_trim_end(&sb));
    TEST_ASSERT_EQUAL_STRING("x", sb.data);

    string_set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, string_trim_end(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
    TEST_ASSERT_TRUE(string_empty(sb));

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_trim_end(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_trim(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "  hello world  ");

    TEST_ASSERT_EQUAL_UINT32(4, string_trim(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, string_length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, string_trim(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);

    string_set(&sb, "\t tabbed \n");
    TEST_ASSERT_EQUAL_UINT32(4, string_trim(&sb));
    TEST_ASSERT_EQUAL_STRING("tabbed", sb.data);

    string_set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, string_trim(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_trim(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_to_upper(void)
{
    string_builder sb = NEW_STRING_BUILDER(20, "hello world 123!");

    TEST_ASSERT_EQUAL_UINT32(16, string_to_upper(&sb));
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD 123!", sb.data);

    string_set(&sb, "MiXeD CaSe");
    TEST_ASSERT_EQUAL_UINT32(10, string_to_upper(&sb));
    TEST_ASSERT_EQUAL_STRING("MIXED CASE", sb.data);

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_to_upper(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_to_lower(void)
{
    string_builder sb = NEW_STRING_BUILDER(20, "HELLO WORLD 123!");

    TEST_ASSERT_EQUAL_UINT32(16, string_to_lower(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world 123!", sb.data);

    string_set(&sb, "MiXeD CaSe");
    TEST_ASSERT_EQUAL_UINT32(10, string_to_lower(&sb));
    TEST_ASSERT_EQUAL_STRING("mixed case", sb.data);

    string_clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, string_to_lower(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_string_join(void)
{
    string parts[3] = {"one", "two", "three"};
    string_builder sb = NEW_STRING_BUILDER(32, "xxxxxxxxx");

    TEST_ASSERT_EQUAL_UINT32(15, string_join(&sb, ", ", 3, parts));
    TEST_ASSERT_EQUAL_STRING("one, two, three", sb.data);
    TEST_ASSERT_EQUAL_UINT32(15, string_length(sb));

    string words[3] = {"a", "b", "c"};
    TEST_ASSERT_EQUAL_UINT32(3, string_join(&sb, "", 3, words));
    TEST_ASSERT_EQUAL_STRING("abc", sb.data);

    string single[1] = {"solo"};
    TEST_ASSERT_EQUAL_UINT32(4, string_join(&sb, "-", 1, single));
    TEST_ASSERT_EQUAL_STRING("solo", sb.data);

    string holes[3] = {"a", "", "c"};
    TEST_ASSERT_EQUAL_UINT32(4, string_join(&sb, "-", 3, holes));
    TEST_ASSERT_EQUAL_STRING("a--c", sb.data);

    string_builder exact = NEW_STRING_BUILDER(8, "");
    string fits[2] = {"123", "56789"};
    TEST_ASSERT_EQUAL_UINT32(8, string_join(&exact, "", 2, fits));
    TEST_ASSERT_EQUAL_STRING("12356789", exact.data);

    string over[1] = {"123456789"};
    TEST_ASSERT_EQUAL_UINT32(0, string_join(&exact, "", 1, over));
}

void test_string_reverse(void)
{
    string_builder sb = NEW_STRING_BUILDER(16, "");
    string_reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("", sb.data);

    string_set(&sb, "a");
    string_reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("a", sb.data);

    string_set(&sb, "ab");
    string_reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("ba", sb.data);

    string_set(&sb, "hello");
    string_reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("olleh", sb.data);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_string_literal_length);
    RUN_TEST(test_string_builder_length);
    RUN_TEST(test_string_builder_capacity);
    RUN_TEST(test_string_literal_empty);
    RUN_TEST(test_string_builder_empty);
    RUN_TEST(test_string_equal_true);
    RUN_TEST(test_string_equal_false);
    RUN_TEST(test_string_equal_builder);
    RUN_TEST(test_string_compare_equal);
    RUN_TEST(test_string_compare_less);
    RUN_TEST(test_string_compare_greater);
    RUN_TEST(test_string_compare_null);
    RUN_TEST(test_string_starts_with_true);
    RUN_TEST(test_string_starts_with_false);
    RUN_TEST(test_string_ends_with_true);
    RUN_TEST(test_string_ends_with_false);
    RUN_TEST(test_string_contains_true);
    RUN_TEST(test_string_contains_false);
    RUN_TEST(test_string_find);
    RUN_TEST(test_string_count);
    RUN_TEST(test_string_set);
    RUN_TEST(test_string_clear);
    RUN_TEST(test_string_append);
    RUN_TEST(test_string_append_char);
    RUN_TEST(test_string_trim_start);
    RUN_TEST(test_string_trim_end);
    RUN_TEST(test_string_trim);
    RUN_TEST(test_string_to_upper);
    RUN_TEST(test_string_to_lower);
    RUN_TEST(test_string_join);
    RUN_TEST(test_string_reverse);
    return UNITY_END();
}
