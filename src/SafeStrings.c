// ────────────────────────────────────────────────────────────────────────────
// SafeStrings — capacity-checked string and mutable string operations.
// ────────────────────────────────────────────────────────────────────────────

#include "SafeStrings.h"
#include <string.h>

u32 StringLiteral_Length(String string)
{
    return (u32)strlen(string);
}

u32 MutableString_Length(MutableString string)
{
    return string.length;
}

u32 String_Capacity(MutableString string)
{
    return string.capacity - string.length;
}

Bool StringLiteral_IsEmpty(String string)
{
    return string[0] == '\0';
}

Bool MutableString_IsEmpty(MutableString string)
{
    return string.length == 0;
}

Bool StringLiteral_Equal(String a, String b)
{
    if( a && b) return 0 == strcmp(a, b);
    return false;
}

i8 StringLiteral_Compare(String a, String b)
{
    if(NULL == a) return -1;
    if(NULL == b) return 1;
    return (i8)strcmp(a, b);
}

Bool StringLiteral_StartsWith(String string, String prefix)
{
    ASSERT_OR(string && prefix) return false;
    return 0 == strncmp(string, prefix, strlen(prefix));
}

Bool StringLiteral_EndsWith(String string, String suffix)
{
    ASSERT_OR(string && suffix) return false;

    u32 suffix_len = StringLiteral_Length(suffix);
    u32 start = StringLiteral_Length(string);
    if(start < suffix_len) return false;
    else start -= suffix_len;
    return 0 == strncmp(string + start, suffix, suffix_len);
}

Bool StringLiteral_Contains(String string, String needle)
{
    ASSERT_OR(string && needle) return false;
    ASSERT_OR(needle[0] != '\0') return false;
    return 0 != strstr(string, needle);
}

u32 StringLiteral_Find(String string, String needle)
{
    ASSERT_OR(string && needle) return 0;
    ASSERT_OR(needle[0] != '\0') return 0;

    char const *pos = strstr(string, needle);
    if(NULL == pos) return 0;
    return (u32)(pos - string);
}

u32 StringLiteral_Count(String string, String needle)
{
    ASSERT_OR(string && needle) return 0;
    ASSERT_OR(needle[0] != '\0') return 0;

    u32 count = 0;
    char const *pos = string;
    while(1)
    {
        pos = strstr(pos, needle);
        if(NULL == pos) break;
        count++;
        pos += strlen(needle);
    }
    return count;
}

u32 String_Set(MutableString *string, String value)
{
    ASSERT_OR(string && string->data && value) return 0;
    u32 value_len = StringLiteral_Length(value);
    if(value_len > string->capacity) return 0;

    memcpy(string->data, value, value_len);
    string->data[value_len] = '\0';
    string->length = value_len;
    return string->length;
}

void String_Clear(MutableString *string)
{
    ASSERT_OR(string) return;
    string->data[0] = '\0';
    string->length = 0;
}

Bool String_Append(MutableString *string, String text)
{
    ASSERT_OR(string && string->data && text) return false;
    u32 value_len = StringLiteral_Length(text);
    if(string->length + value_len > string->capacity) return false;

    memcpy(string->data + string->length, text, value_len);
    string->length += value_len;
    string->data[string->length] = '\0';
    return true;
}

Bool String_AppendChar(MutableString *string, ASCII character)
{
    ASSERT_OR(string && string->data) return false;
    if(string->length >= string->capacity) return false;

    string->data[string->length] = character;
    string->length++;
    string->data[string->length] = '\0';
    return true;
}

u32 String_Join(MutableString *string, String separator, u8 number_of_joins, String list[number_of_joins])
{
    ASSERT_OR(string && string->data && separator && list) return 0;
    ASSERT_OR(list[0]) return 0;

    u32 length = 0;
    length += StringLiteral_Length(list[0]);
    for(u16 i = 1; i < number_of_joins; i++)
    {
        ASSERT_OR(list[i]) return 0;
        length += StringLiteral_Length(separator);
        length += StringLiteral_Length(list[i]);
    }

    if(length <= string->capacity)
    {
        String_Set(string, list[0]);
        for(u16 i = 1; i < number_of_joins; i++)
        {
            String_Append(string, separator);
            String_Append(string, list[i]);
        }
        return length;
    }
    return 0;
}

u8 String_Split(MutableString *string, String delimiter, u8 number_of_splits, String splits[number_of_splits])
{
    ASSERT_OR(string && string->data && splits) return 0;
    ASSERT_OR(delimiter && number_of_splits > 0) return 0;

    if('\0' == delimiter[0])
    {
        if(0 == string->length)
        {
            for(u8 i = 0; i < number_of_splits; i++) splits[i] = "";
            return 0;
        }
        splits[0] = string->data;
        for(u8 i = 1; i < number_of_splits; i++) splits[i] = "";
        return 1;
    }

    u8 count = 0;
    char *pos = string->data;

    pos += strspn(pos, delimiter);
    while('\0' != *pos && count < number_of_splits)
    {
        splits[count++] = pos;
        pos += strcspn(pos, delimiter);
        if('\0' != *pos)
        {
            *pos = '\0';
            pos++;
            pos += strspn(pos, delimiter);
        }
    }
    for(u8 i = count; i < number_of_splits; i++) splits[i] = "";
    return count;
}

u32 String_Insert(MutableString *string, u32 position, String insert)
{
    ASSERT_OR(string && string->data && insert) return 0;
    ASSERT_OR(position <= string->length) return 0;

    u32 insert_len = StringLiteral_Length(insert);
    if(0 == insert_len) return 0;
    if(String_Capacity(*string) < insert_len) return 0;

    u32 shift_len = string->length - position;
    if(shift_len) memmove(&string->data[position + insert_len], &string->data[position], shift_len);
    memcpy(&string->data[position], insert, insert_len);
    string->length += insert_len;
    string->data[string->length] = '\0';
    return insert_len;
}

u32 String_Replace(MutableString *string, String search, String replace)
{
    ASSERT_OR(string && string->data && search && replace) return 0;
    ASSERT_OR('\0' != search[0]) return 0;
    if(!StringLiteral_Contains(string->data, search)) return 0;

    u32 spot = StringLiteral_Find(string->data, search);
    u32 search_len = StringLiteral_Length(search);
    u32 replace_len = StringLiteral_Length(replace);
    u32 tail_len = string->length - spot - search_len;

    if(replace_len > search_len)
    {
        // Expand
        u32 diff = replace_len - search_len;
        if(diff > String_Capacity(*string)) return 0;
        memmove(&string->data[spot + replace_len], &string->data[spot + search_len], tail_len + 1);
        memcpy(&string->data[spot], replace, replace_len);
        string->length += diff;
    }
    else if(replace_len < search_len)
    {
        // Shrink (always fits)
        u32 diff = search_len - replace_len;
        if(replace_len) memcpy(&string->data[spot], replace, replace_len);
        memmove(&string->data[spot + replace_len], &string->data[spot + search_len], tail_len + 1);
        string->length -= diff;
    }
    else
    {
        memcpy(&string->data[spot], replace, replace_len);
    }
    return 1;
}

void String_Reverse(MutableString *string)
{
    ASSERT_OR(string && string->data) return;
    u32 len = string->length;
    if(len < 2) return;
    for(u32 i = 0, j = len - 1; i < j; i++, j--)
    {
        char temp = string->data[i];
        string->data[i] = string->data[j];
        string->data[j] = temp;
    }
}

u32 String_TrimStart(MutableString *string)
{
    ASSERT_OR(string) return 0;

    u32 start = 0;
    while(ASCII_IsSpace(string->data[start])) start++;
    string->length -= start;
    memmove(string->data, &string->data[start], string->length);
    string->data[string->length] = '\0';

    return start;
}

u32 String_TrimEnd(MutableString *string)
{
    ASSERT_OR(string) return 0;

    u32 initial_length = string->length;
    while(string->length)
    {
        if(ASCII_IsSpace(string->data[string->length - 1]))
        {
            string->length--;
        }
        else
        {
            break;
        }
    }
    string->data[string->length] = '\0';
    return initial_length - string->length;
}

u32 String_Trim(MutableString *string)
{
    return String_TrimStart(string) + String_TrimEnd(string);
}

u32 String_ToUpper(MutableString *string)
{
    u32 i;
    for(i = 0; i < string->length; i++)
    {
        string->data[i] = ASCII_ToUpper(string->data[i]);
    }
    return i;
}
u32 String_ToLower(MutableString *string)
{
    u32 i;
    for(i = 0; i < string->length; i++)
    {
        string->data[i] = ASCII_ToLower(string->data[i]);
    }
    return i;
}