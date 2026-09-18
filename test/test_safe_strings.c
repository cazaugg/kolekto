#include "unity.h"
#include "../src/safe_string.h"

void test_String_Length(void);
void test_MutableString_Length(void);
void test_String_Capacity(void);
void test_String_IsEmpty(void);
void test_MutableString_IsEmpty(void);
void test_String_Equal_True(void);
void test_String_Equal_False(void);
void test_String_Equal_Builder(void);
void test_String_Compare_Equal(void);
void test_String_Compare_Less(void);
void test_String_Compare_Greater(void);
void test_String_Compare_Null(void);
void test_String_StartsWith_True(void);
void test_String_StartsWith_False(void);
void test_String_EndsWith_True(void);
void test_String_EndsWith_False(void);
void test_String_Contains_True(void);
void test_String_Contains_False(void);
void test_String_Find(void);
void test_String_Count(void);
void test_String_Set(void);
void test_String_Clear(void);
void test_String_Append(void);
void test_String_AppendChar(void);
void test_String_TrimStart(void);
void test_String_TrimEnd(void);
void test_String_Trim(void);
void test_String_ToUpper(void);
void test_String_ToLower(void);
void test_String_Join(void);
void test_String_Reverse(void);
void test_String_Split(void);
void test_String_Split_NoDelimiter(void);
void test_String_Split_Empty(void);
void test_String_Split_Limit(void);
void test_String_Split_RepeatedDelimiters(void);
void test_String_Split_MultiDelimiter(void);
void test_String_Insert(void);
void test_String_Insert_Empty(void);
void test_String_Insert_ExactCapacity(void);
void test_String_Insert_InsufficientCapacity(void);
void test_String_Replace_SameLength(void);
void test_String_Replace_Longer(void);
void test_String_Replace_Shorter(void);
void test_String_Replace_Remove(void);
void test_String_Replace_NotFound(void);
void test_String_Replace_ExactCapacity(void);
void test_String_Replace_InsufficientCapacity(void);

void setUp(void)
{
}

void tearDown(void)
{
}

void test_String_Length(void)
{
    String empty = "";
    String word = "abcd";
    String phrase = "hello world";
    TEST_ASSERT_EQUAL_UINT32(0, String_Length(empty));
    TEST_ASSERT_EQUAL_UINT32(4, String_Length(word));
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(phrase));
}

void test_MutableString_Length(void)
{
    MutableString sb = NEW_MUTABLE_STRING(32, "");
    TEST_ASSERT_EQUAL_UINT32(0, String_Length(sb));

    String_Set(&sb, "hello");
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));

    String_Append(&sb, " world");
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(sb));

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_Length(sb));
}

void test_String_Capacity(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "abc");

    TEST_ASSERT_EQUAL_UINT32(13, String_Capacity(sb));

    String_Append(&sb, "123456789");
    TEST_ASSERT_EQUAL_UINT32(4, String_Capacity(sb));

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(16, String_Capacity(sb));
}

void test_String_IsEmpty(void)
{
    String empty = "";
    String single = "a";
    String word = "hello";
    TEST_ASSERT_TRUE(String_IsEmpty(empty));
    TEST_ASSERT_FALSE(String_IsEmpty(single));
    TEST_ASSERT_FALSE(String_IsEmpty(word));
}

void test_MutableString_IsEmpty(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "");
    TEST_ASSERT_TRUE(String_IsEmpty(sb));

    String_Set(&sb, "x");
    TEST_ASSERT_FALSE(String_IsEmpty(sb));

    String_Clear(&sb);
    TEST_ASSERT_TRUE(String_IsEmpty(sb));
}

void test_String_Equal_True(void)
{
    String empty = "";
    String word = "abcd";
    MutableString phrase = NEW_MUTABLE_STRING(20, "hello world");
    TEST_ASSERT_TRUE(String_Equal(empty, ""));
    TEST_ASSERT_TRUE(String_Equal(word, "abcd"));
    TEST_ASSERT_TRUE(String_Equal(phrase, "hello world"));
}

void test_String_Equal_False(void)
{
    String abc = "abc";
    MutableString phrase = NEW_MUTABLE_STRING(20, "abd");
    String empty = "";
    TEST_ASSERT_FALSE(String_Equal(abc, "abd"));
    TEST_ASSERT_FALSE(String_Equal(abc, ""));
    TEST_ASSERT_FALSE(String_Equal(empty, "abc"));
    TEST_ASSERT_FALSE(String_Equal(phrase, "abc"));
}

void test_String_Equal_Builder(void)
{
    char buffer[16] = {0};
    MutableString sb = {.data = buffer, .capacity = 16, .length = 0};
    String hello = "hello";
    String world = "world";

    String_Set(&sb, hello);
    TEST_ASSERT_TRUE(String_Equal(sb, hello));
    TEST_ASSERT_TRUE(String_Equal(hello, sb));
    TEST_ASSERT_FALSE(String_Equal(sb, world));

    char buffer2[16] = {0};
    MutableString sb2 = {.data = buffer2, .capacity = 16, .length = 0};
    String_Set(&sb2, hello);
    TEST_ASSERT_TRUE(String_Equal(sb, sb2));
}

void test_String_Compare_Equal(void)
{
    String a = "hello";
    String b = "hello";
    String empty = "";
    MutableString c = NEW_MUTABLE_STRING(20, "hello");
    TEST_ASSERT_EQUAL_INT8(0, String_Compare(a, b));
    TEST_ASSERT_EQUAL_INT8(0, String_Compare(a, c));
    TEST_ASSERT_EQUAL_INT8(0, String_Compare(empty, empty));
}

void test_String_Compare_Less(void)
{
    String a = "abc";
    String b = "abd";
    MutableString c = NEW_MUTABLE_STRING(20, "hello");
    String prefix = "ab";
    TEST_ASSERT_TRUE(String_Compare(a, b) < 0);
    TEST_ASSERT_TRUE(String_Compare(c, b) > 0);
    TEST_ASSERT_TRUE(String_Compare(prefix, a) < 0);
}

void test_String_Compare_Greater(void)
{
    String a = "abd";
    String b = "abc";
    MutableString c = NEW_MUTABLE_STRING(20, "hello");
    String longer = "abcd";
    TEST_ASSERT_TRUE(String_Compare(a, b) > 0);
    TEST_ASSERT_TRUE(String_Compare(a, c) < 0);
    TEST_ASSERT_TRUE(String_Compare(longer, "abc") > 0);
}

void test_String_Compare_Null(void)
{
    TEST_ASSERT_EQUAL_INT8(-1, StringLiteral_Compare(NULL, "a"));
    TEST_ASSERT_EQUAL_INT8(1, StringLiteral_Compare("a", NULL));
    TEST_ASSERT_EQUAL_INT8(-1, StringLiteral_Compare(NULL, NULL));
}

void test_String_StartsWith_True(void)
{
    String sentence = "hello world";
    TEST_ASSERT_TRUE(String_StartsWith(sentence, "hello"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence, "h"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence, "hello world"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence, ""));

    MutableString sentence2 = NEW_MUTABLE_STRING(11, "hello world");
    TEST_ASSERT_TRUE(String_StartsWith(sentence2, "hello"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence2, "h"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence2, "hello world"));
    TEST_ASSERT_TRUE(String_StartsWith(sentence2, ""));
}

void test_String_StartsWith_False(void)
{
    String sentence = "hello world";
    TEST_ASSERT_FALSE(String_StartsWith(sentence, "world"));
    TEST_ASSERT_FALSE(String_StartsWith(sentence, "HELLO"));
    TEST_ASSERT_FALSE(String_StartsWith(sentence, "hello world!"));

    MutableString sentence2 = NEW_MUTABLE_STRING(11, "hello world");
    TEST_ASSERT_FALSE(String_StartsWith(sentence2, "world"));
    TEST_ASSERT_FALSE(String_StartsWith(sentence2, "HELLO"));
    TEST_ASSERT_FALSE(String_StartsWith(sentence2, "hello world!"));
}

void test_String_EndsWith_True(void)
{
    String sentence = "hello world";
    TEST_ASSERT_TRUE(String_EndsWith(sentence, "world"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence, "d"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence, "hello world"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence, ""));

    MutableString sentence2 = NEW_MUTABLE_STRING(11, "hello world");
    TEST_ASSERT_TRUE(String_EndsWith(sentence2, "world"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence2, "d"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence2, "hello world"));
    TEST_ASSERT_TRUE(String_EndsWith(sentence2, ""));
}

void test_String_EndsWith_False(void)
{
    String sentence = "hello world";
    TEST_ASSERT_FALSE(String_EndsWith(sentence, "hello"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence, "WORLD"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence, "xhello world"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence, "hello world!"));
    
    MutableString sentence2 = NEW_MUTABLE_STRING(11, "hello world");
    TEST_ASSERT_FALSE(String_EndsWith(sentence2, "hello"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence2, "WORLD"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence2, "xhello world"));
    TEST_ASSERT_FALSE(String_EndsWith(sentence2, "hello world!"));
}

void test_String_Contains_True(void)
{
    String sentence = "hello world";
    TEST_ASSERT_TRUE(String_Contains(sentence, "world"));
    TEST_ASSERT_TRUE(String_Contains(sentence, "hello"));
    TEST_ASSERT_TRUE(String_Contains(sentence, "lo wo"));
    TEST_ASSERT_TRUE(String_Contains(sentence, " "));
}

void test_String_Contains_False(void)
{
    String sentence = "hello world";
    TEST_ASSERT_FALSE(String_Contains(sentence, "xyz"));
    TEST_ASSERT_FALSE(String_Contains(sentence, "WORLD"));
    TEST_ASSERT_FALSE(String_Contains(sentence, "hello world!"));
}

void test_String_Find(void)
{
    String sentence = "hello world";
    TEST_ASSERT_EQUAL_UINT32(0, String_Find(sentence, "hello"));
    TEST_ASSERT_EQUAL_UINT32(6, String_Find(sentence, "world"));
    TEST_ASSERT_EQUAL_UINT32(4, String_Find(sentence, "o"));
    TEST_ASSERT_EQUAL_UINT32(4, String_Find(sentence, "o w"));
    TEST_ASSERT_EQUAL_UINT32(0, String_Find(sentence, "hello world"));
}

void test_String_Count(void)
{
    String sentence = "hello world";
    TEST_ASSERT_EQUAL_UINT32(3, String_Count(sentence, "l"));
    TEST_ASSERT_EQUAL_UINT32(2, String_Count(sentence, "o"));
    TEST_ASSERT_EQUAL_UINT32(1, String_Count(sentence, "world"));
    TEST_ASSERT_EQUAL_UINT32(0, String_Count("abc", "z"));
    TEST_ASSERT_EQUAL_UINT32(1, String_Count("aaa", "aaa"));
    TEST_ASSERT_EQUAL_UINT32(2, String_Count("aba", "a"));
}

void test_String_Set(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");

    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    String_Set(&sb, "hi");
    TEST_ASSERT_EQUAL_UINT32(2, String_Length(sb));
    TEST_ASSERT_EQUAL_STRING("hi", sb.data);

    String_Set(&sb, "");
    TEST_ASSERT_EQUAL_UINT32(0, String_Length(sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_Clear(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");
    TEST_ASSERT_FALSE(String_IsEmpty(sb));

    String_Clear(&sb);
    TEST_ASSERT_TRUE(String_IsEmpty(sb));
    TEST_ASSERT_EQUAL_UINT32(0, String_Length(sb));
}

void test_String_Append(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");

    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));

    TEST_ASSERT_TRUE(String_Append(&sb, " world"));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(sb));

    String_Clear(&sb);
    TEST_ASSERT_TRUE(String_Append(&sb, "abc"));
    TEST_ASSERT_EQUAL_STRING("abc", sb.data);
}

void test_String_AppendChar(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "abc ");

    TEST_ASSERT_TRUE(String_AppendChar(&sb, 'h'));
    TEST_ASSERT_TRUE(String_AppendChar(&sb, 'i'));
    TEST_ASSERT_EQUAL_STRING("abc hi", sb.data);
    TEST_ASSERT_EQUAL_UINT32(6, String_Length(sb));

    String_AppendChar(&sb, '!');
    TEST_ASSERT_EQUAL_STRING("abc hi!", sb.data);
}

void test_String_TrimStart(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "   hello");

    TEST_ASSERT_EQUAL_UINT32(3, String_TrimStart(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, String_TrimStart(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    String_Set(&sb, "\t\n \r\v\f x");
    TEST_ASSERT_EQUAL_UINT32(7, String_TrimStart(&sb));
    TEST_ASSERT_EQUAL_STRING("x", sb.data);

    String_Set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, String_TrimStart(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
    TEST_ASSERT_TRUE(String_IsEmpty(sb));

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_TrimStart(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_TrimEnd(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello   ");

    TEST_ASSERT_EQUAL_UINT32(3, String_TrimEnd(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, String_TrimEnd(&sb));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);

    String_Set(&sb, "x \t\n \r\v\f");
    TEST_ASSERT_EQUAL_UINT32(7, String_TrimEnd(&sb));
    TEST_ASSERT_EQUAL_STRING("x", sb.data);

    String_Set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, String_TrimEnd(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
    TEST_ASSERT_TRUE(String_IsEmpty(sb));

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_TrimEnd(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_Trim(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "  hello world  ");

    TEST_ASSERT_EQUAL_UINT32(4, String_Trim(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(sb));

    TEST_ASSERT_EQUAL_UINT32(0, String_Trim(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);

    String_Set(&sb, "\t tabbed \n");
    TEST_ASSERT_EQUAL_UINT32(4, String_Trim(&sb));
    TEST_ASSERT_EQUAL_STRING("tabbed", sb.data);

    String_Set(&sb, "   ");
    TEST_ASSERT_EQUAL_UINT32(3, String_Trim(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_Trim(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_ToUpper(void)
{
    MutableString sb = NEW_MUTABLE_STRING(20, "hello world 123!");

    TEST_ASSERT_EQUAL_UINT32(16, String_ToUpper(&sb));
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD 123!", sb.data);

    String_Set(&sb, "MiXeD CaSe");
    TEST_ASSERT_EQUAL_UINT32(10, String_ToUpper(&sb));
    TEST_ASSERT_EQUAL_STRING("MIXED CASE", sb.data);

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_ToUpper(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_ToLower(void)
{
    MutableString sb = NEW_MUTABLE_STRING(20, "HELLO WORLD 123!");

    TEST_ASSERT_EQUAL_UINT32(16, String_ToLower(&sb));
    TEST_ASSERT_EQUAL_STRING("hello world 123!", sb.data);

    String_Set(&sb, "MiXeD CaSe");
    TEST_ASSERT_EQUAL_UINT32(10, String_ToLower(&sb));
    TEST_ASSERT_EQUAL_STRING("mixed case", sb.data);

    String_Clear(&sb);
    TEST_ASSERT_EQUAL_UINT32(0, String_ToLower(&sb));
    TEST_ASSERT_EQUAL_STRING("", sb.data);
}

void test_String_Join(void)
{
    String parts[3] = {"one", "two", "three"};
    MutableString sb = NEW_MUTABLE_STRING(32, "xxxxxxxxx");

    TEST_ASSERT_EQUAL_UINT32(15, String_Join(&sb, ", ", 3, parts));
    TEST_ASSERT_EQUAL_STRING("one, two, three", sb.data);
    TEST_ASSERT_EQUAL_UINT32(15, String_Length(sb));

    String words[3] = {"a", "b", "c"};
    TEST_ASSERT_EQUAL_UINT32(3, String_Join(&sb, "", 3, words));
    TEST_ASSERT_EQUAL_STRING("abc", sb.data);

    String single[1] = {"solo"};
    TEST_ASSERT_EQUAL_UINT32(4, String_Join(&sb, "-", 1, single));
    TEST_ASSERT_EQUAL_STRING("solo", sb.data);

    String holes[3] = {"a", "", "c"};
    TEST_ASSERT_EQUAL_UINT32(4, String_Join(&sb, "-", 3, holes));
    TEST_ASSERT_EQUAL_STRING("a--c", sb.data);

    MutableString exact = NEW_MUTABLE_STRING(8, "");
    String fits[2] = {"123", "56789"};
    TEST_ASSERT_EQUAL_UINT32(8, String_Join(&exact, "", 2, fits));
    TEST_ASSERT_EQUAL_STRING("12356789", exact.data);

    String over[1] = {"123456789"};
    TEST_ASSERT_EQUAL_UINT32(0, String_Join(&exact, "", 1, over));
}

void test_String_Reverse(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "");
    String_Reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("", sb.data);

    String_Set(&sb, "a");
    String_Reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("a", sb.data);

    String_Set(&sb, "ab");
    String_Reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("ba", sb.data);

    String_Set(&sb, "hello");
    String_Reverse(&sb);
    TEST_ASSERT_EQUAL_STRING("olleh", sb.data);
}

void test_String_Split(void)
{
    MutableString sb = NEW_MUTABLE_STRING(32, "one,two,three");
    String splits[3];

    TEST_ASSERT_EQUAL_UINT8(3, String_Split(&sb, ",", 3, splits));
    TEST_ASSERT_EQUAL_STRING("one", splits[0]);
    TEST_ASSERT_EQUAL_STRING("two", splits[1]);
    TEST_ASSERT_EQUAL_STRING("three", splits[2]);
}

void test_String_Split_NoDelimiter(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");
    String splits[2];

    TEST_ASSERT_EQUAL_UINT8(1, String_Split(&sb, ",", 2, splits));
    TEST_ASSERT_EQUAL_STRING("hello", splits[0]);
    TEST_ASSERT_EQUAL_STRING("", splits[1]);
}

void test_String_Split_Empty(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "");
    String splits[2] = {"x", "y"};

    TEST_ASSERT_EQUAL_UINT8(0, String_Split(&sb, ",", 2, splits));
    TEST_ASSERT_EQUAL_STRING("", splits[0]);
    TEST_ASSERT_EQUAL_STRING("", splits[1]);
}

void test_String_Split_Limit(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "a,b,c,d");
    String splits[2];

    TEST_ASSERT_EQUAL_UINT8(2, String_Split(&sb, ",", 2, splits));
    TEST_ASSERT_EQUAL_STRING("a", splits[0]);
    TEST_ASSERT_EQUAL_STRING("b", splits[1]);
}

void test_String_Split_RepeatedDelimiters(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "a,,b,,c");
    String splits[3];

    TEST_ASSERT_EQUAL_UINT8(3, String_Split(&sb, ",", 3, splits));
    TEST_ASSERT_EQUAL_STRING("a", splits[0]);
    TEST_ASSERT_EQUAL_STRING("b", splits[1]);
    TEST_ASSERT_EQUAL_STRING("c", splits[2]);

    String_Set(&sb, ",a,b,");
    TEST_ASSERT_EQUAL_UINT8(2, String_Split(&sb, ",", 2, splits));
    TEST_ASSERT_EQUAL_STRING("a", splits[0]);
    TEST_ASSERT_EQUAL_STRING("b", splits[1]);
}

void test_String_Split_MultiDelimiter(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "a,b;c d");
    String splits[4];

    TEST_ASSERT_EQUAL_UINT8(4, String_Split(&sb, ",; ", 4, splits));
    TEST_ASSERT_EQUAL_STRING("a", splits[0]);
    TEST_ASSERT_EQUAL_STRING("b", splits[1]);
    TEST_ASSERT_EQUAL_STRING("c", splits[2]);
    TEST_ASSERT_EQUAL_STRING("d", splits[3]);
}

void test_String_Insert(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");

    TEST_ASSERT_EQUAL_UINT32(1, String_Insert(&sb, 5, "!"));
    TEST_ASSERT_EQUAL_STRING("hello!", sb.data);
    TEST_ASSERT_EQUAL_UINT32(6, String_Length(sb));

    TEST_ASSERT_EQUAL_UINT32(1, String_Insert(&sb, 0, "X"));
    TEST_ASSERT_EQUAL_STRING("Xhello!", sb.data);

    TEST_ASSERT_EQUAL_UINT32(3, String_Insert(&sb, 1, "abc"));
    TEST_ASSERT_EQUAL_STRING("Xabchello!", sb.data);
    TEST_ASSERT_EQUAL_UINT32(10, String_Length(sb));
}

void test_String_Insert_Empty(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");

    TEST_ASSERT_EQUAL_UINT32(0, String_Insert(&sb, 2, ""));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));
}

void test_String_Insert_ExactCapacity(void)
{
    MutableString sb = NEW_MUTABLE_STRING(8, "abcd");

    TEST_ASSERT_EQUAL_UINT32(4, String_Insert(&sb, 4, "efgh"));
    TEST_ASSERT_EQUAL_STRING("abcdefgh", sb.data);
    TEST_ASSERT_EQUAL_UINT32(8, String_Length(sb));
}

void test_String_Insert_InsufficientCapacity(void)
{
    MutableString sb = NEW_MUTABLE_STRING(8, "abcd");

    TEST_ASSERT_EQUAL_UINT32(0, String_Insert(&sb, 2, "12345"));
    TEST_ASSERT_EQUAL_STRING("abcd", sb.data);
    TEST_ASSERT_EQUAL_UINT32(4, String_Length(sb));
}

void test_String_Replace_SameLength(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello world");

    TEST_ASSERT_EQUAL_UINT32(1, String_Replace(&sb, "world", "there"));
    TEST_ASSERT_EQUAL_STRING("hello there", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(sb));
}

void test_String_Replace_Longer(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hi world");

    TEST_ASSERT_EQUAL_UINT32(1, String_Replace(&sb, "hi", "hello"));
    TEST_ASSERT_EQUAL_STRING("hello world", sb.data);
    TEST_ASSERT_EQUAL_UINT32(11, String_Length(sb));
}

void test_String_Replace_Shorter(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello world");

    TEST_ASSERT_EQUAL_UINT32(1, String_Replace(&sb, "world", "you"));
    TEST_ASSERT_EQUAL_STRING("hello you", sb.data);
    TEST_ASSERT_EQUAL_UINT32(9, String_Length(sb));
}

void test_String_Replace_Remove(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello world");

    TEST_ASSERT_EQUAL_UINT32(1, String_Replace(&sb, " world", ""));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));
}

void test_String_Replace_NotFound(void)
{
    MutableString sb = NEW_MUTABLE_STRING(16, "hello");

    TEST_ASSERT_EQUAL_UINT32(0, String_Replace(&sb, "xyz", "abc"));
    TEST_ASSERT_EQUAL_STRING("hello", sb.data);
    TEST_ASSERT_EQUAL_UINT32(5, String_Length(sb));
}

void test_String_Replace_ExactCapacity(void)
{
    MutableString sb = NEW_MUTABLE_STRING(8, "hixxx");

    TEST_ASSERT_EQUAL_UINT32(1, String_Replace(&sb, "hi", "hello"));
    TEST_ASSERT_EQUAL_STRING("helloxxx", sb.data);
    TEST_ASSERT_EQUAL_UINT32(8, String_Length(sb));
}

void test_String_Replace_InsufficientCapacity(void)
{
    MutableString sb = NEW_MUTABLE_STRING(8, "hi");

    TEST_ASSERT_EQUAL_UINT32(0, String_Replace(&sb, "hi", "hello world"));
    TEST_ASSERT_EQUAL_STRING("hi", sb.data);
    TEST_ASSERT_EQUAL_UINT32(2, String_Length(sb));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_String_Length);
    RUN_TEST(test_MutableString_Length);
    RUN_TEST(test_String_Capacity);
    RUN_TEST(test_String_IsEmpty);
    RUN_TEST(test_MutableString_IsEmpty);
    RUN_TEST(test_String_Equal_True);
    RUN_TEST(test_String_Equal_False);
    RUN_TEST(test_String_Equal_Builder);
    RUN_TEST(test_String_Compare_Equal);
    RUN_TEST(test_String_Compare_Less);
    RUN_TEST(test_String_Compare_Greater);
    RUN_TEST(test_String_Compare_Null);
    RUN_TEST(test_String_StartsWith_True);
    RUN_TEST(test_String_StartsWith_False);
    RUN_TEST(test_String_EndsWith_True);
    RUN_TEST(test_String_EndsWith_False);
    RUN_TEST(test_String_Contains_True);
    RUN_TEST(test_String_Contains_False);
    RUN_TEST(test_String_Find);
    RUN_TEST(test_String_Count);
    RUN_TEST(test_String_Set);
    RUN_TEST(test_String_Clear);
    RUN_TEST(test_String_Append);
    RUN_TEST(test_String_AppendChar);
    RUN_TEST(test_String_TrimStart);
    RUN_TEST(test_String_TrimEnd);
    RUN_TEST(test_String_Trim);
    RUN_TEST(test_String_ToUpper);
    RUN_TEST(test_String_ToLower);
    RUN_TEST(test_String_Join);
    RUN_TEST(test_String_Reverse);
    RUN_TEST(test_String_Split);
    RUN_TEST(test_String_Split_NoDelimiter);
    RUN_TEST(test_String_Split_Empty);
    RUN_TEST(test_String_Split_Limit);
    RUN_TEST(test_String_Split_RepeatedDelimiters);
    RUN_TEST(test_String_Split_MultiDelimiter);
    RUN_TEST(test_String_Insert);
    RUN_TEST(test_String_Insert_Empty);
    RUN_TEST(test_String_Insert_ExactCapacity);
    RUN_TEST(test_String_Insert_InsufficientCapacity);
    RUN_TEST(test_String_Replace_SameLength);
    RUN_TEST(test_String_Replace_Longer);
    RUN_TEST(test_String_Replace_Shorter);
    RUN_TEST(test_String_Replace_Remove);
    RUN_TEST(test_String_Replace_NotFound);
    RUN_TEST(test_String_Replace_ExactCapacity);
    RUN_TEST(test_String_Replace_InsufficientCapacity);
    return UNITY_END();
}
