#include "safe_string.h"
#include <string.h>

u32 StringLiteral_Length(String str)
{
    return (u32)strlen(str);
}

u32 MutableString_Length(MutableString str)
{
    return str.length;
}

u32 String_Capacity(MutableString str)
{
    return str.capacity - str.length;
}

Bool StringLiteral_IsEmpty(String str)
{
    return str[0] == '\0';
}

Bool MutableString_IsEmpty(MutableString str)
{
    return str.length == 0;
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

Bool StringLiteral_StartsWith(String str, String prefix)
{
    ASSERT_OR(str && prefix) return false;
    return 0 == strncmp(str, prefix, strlen(prefix));
}

Bool StringLiteral_EndsWith(String str, String suffix)
{
    ASSERT_OR(str && suffix) return false;

    u32 suffix_len = StringLiteral_Length(suffix);
    u32 start = StringLiteral_Length(str);
    if(start < suffix_len) return false;
    else start -= suffix_len;
    return 0 == strncmp(str + start, suffix, suffix_len);
}

Bool StringLiteral_Contains(String str, String needle)
{
    ASSERT_OR(str && needle) return false;
    ASSERT_OR(needle[0] != '\0') return false;
    return 0 != strstr(str, needle);
}

u32 StringLiteral_Find(String str, String needle)
{
    ASSERT_OR(str && needle) return 0;
    ASSERT_OR(needle[0] != '\0') return 0;

    char const *pos = strstr(str, needle);
    if(NULL == pos) return 0;
    return (u32)(pos - str);
}

u32 StringLiteral_Count(String str, String needle)
{
    ASSERT_OR(str && needle) return 0;
    ASSERT_OR(needle[0] != '\0') return 0;

    u32 count = 0;
    char const *pos = str;
    while(1)
    {
        pos = strstr(pos, needle);
        if(NULL == pos) break;
        count++;
        pos += strlen(needle);
    }
    return count;
}

u32 String_Set(MutableString *str, String value)
{
    ASSERT_OR(str && str->data && value) return 0;
    u32 value_len = StringLiteral_Length(value);
    if(value_len > str->capacity) return 0;

    memcpy(str->data, value, value_len);
    str->data[value_len] = '\0';
    str->length = value_len;
    return str->length;
}

void String_Clear(MutableString *str)
{
    ASSERT_OR(str) return;
    str->data[0] = '\0';
    str->length = 0;
}

Bool String_Append(MutableString *str, String text)
{
    ASSERT_OR(str && str->data && text) return false;
    u32 value_len = StringLiteral_Length(text);
    if(str->length + value_len > str->capacity) return false;

    memcpy(str->data + str->length, text, value_len);
    str->length += value_len;
    str->data[str->length] = '\0';
    return true;
}

Bool String_AppendChar(MutableString *str, ASCII ch)
{
    ASSERT_OR(str && str->data) return false;
    if(str->length >= str->capacity) return false;

    str->data[str->length] = ch;
    str->length++;
    str->data[str->length] = '\0';
    return true;
}

u32 String_Join(MutableString *str, String separator, u8 nof_joins, String list[nof_joins])
{
    ASSERT_OR(str && str->data && separator && list) return 0;
    ASSERT_OR(list[0]) return 0;

    u32 length = 0;
    length += StringLiteral_Length(list[0]);
    for(u16 i = 1; i < nof_joins; i++)
    {
        ASSERT_OR(list[i]) return 0;
        length += StringLiteral_Length(separator);
        length += StringLiteral_Length(list[i]);
    }

    if(length <= str->capacity)
    {
        String_Set(str, list[0]);
        for(u16 i = 1; i < nof_joins; i++)
        {
            String_Append(str, separator);
            String_Append(str, list[i]);
        }
        return length;
    }
    return 0;
}

u8 String_Split(MutableString *str, String delimiter, u8 nof_splits, String splits[nof_splits])
{
    ASSERT_OR(str && str->data && splits) return 0;
    ASSERT_OR(delimiter && nof_splits > 0) return 0;

    if('\0' == delimiter[0])
    {
        if(0 == str->length)
        {
            for(u8 i = 0; i < nof_splits; i++) splits[i] = "";
            return 0;
        }
        splits[0] = str->data;
        for(u8 i = 1; i < nof_splits; i++) splits[i] = "";
        return 1;
    }

    u8 count = 0;
    char *pos = str->data;

    pos += strspn(pos, delimiter);
    while('\0' != *pos && count < nof_splits)
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
    for(u8 i = count; i < nof_splits; i++) splits[i] = "";
    return count;
}

u32 String_Insert(MutableString *str, u32 position, String insert)
{
    ASSERT_OR(str && str->data && insert) return 0;
    ASSERT_OR(position <= str->length) return 0;

    u32 insert_len = StringLiteral_Length(insert);
    if(0 == insert_len) return 0;
    if(String_Capacity(*str) < insert_len) return 0;

    u32 shift_len = str->length - position;
    if(shift_len) memmove(&str->data[position + insert_len], &str->data[position], shift_len);
    memcpy(&str->data[position], insert, insert_len);
    str->length += insert_len;
    str->data[str->length] = '\0';
    return insert_len;
}

u32 String_Replace(MutableString *str, String search, String replace)
{
    ASSERT_OR(str && str->data && search && replace) return 0;
    ASSERT_OR('\0' != search[0]) return 0;
    if(!StringLiteral_Contains(str->data, search)) return 0;

    u32 spot = StringLiteral_Find(str->data, search);
    u32 search_len = StringLiteral_Length(search);
    u32 replace_len = StringLiteral_Length(replace);
    u32 tail_len = str->length - spot - search_len;

    if(replace_len > search_len)
    {
        // Expand
        u32 diff = replace_len - search_len;
        if(diff > String_Capacity(*str)) return 0;
        memmove(&str->data[spot + replace_len], &str->data[spot + search_len], tail_len + 1);
        memcpy(&str->data[spot], replace, replace_len);
        str->length += diff;
    }
    else if(replace_len < search_len)
    {
        // Shrink (always fits)
        u32 diff = search_len - replace_len;
        if(replace_len) memcpy(&str->data[spot], replace, replace_len);
        memmove(&str->data[spot + replace_len], &str->data[spot + search_len], tail_len + 1);
        str->length -= diff;
    }
    else
    {
        memcpy(&str->data[spot], replace, replace_len);
    }
    return 1;
}

void String_Reverse(MutableString *str)
{
    ASSERT_OR(str && str->data) return;
    u32 len = str->length;
    if(len < 2) return;
    for(u32 i = 0, j = len - 1; i < j; i++, j--)
    {
        char temp = str->data[i];
        str->data[i] = str->data[j];
        str->data[j] = temp;
    }
}

u32 String_TrimStart(MutableString *str)
{
    ASSERT_OR(str) return 0;

    u32 start = 0;
    while(ASCII_IsSpace(str->data[start])) start++;
    str->length -= start;
    memmove(str->data, &str->data[start], str->length);
    str->data[str->length] = '\0';

    return start;
}

u32 String_TrimEnd(MutableString *str)
{
    ASSERT_OR(str) return 0;

    u32 initial_length = str->length;
    while(str->length)
    {
        if(ASCII_IsSpace(str->data[str->length - 1]))
        {
            str->length--;
        }
        else
        {
            break;
        }
    }
    str->data[str->length] = '\0';
    return initial_length - str->length;
}

u32 String_Trim(MutableString *str)
{
    return String_TrimStart(str) + String_TrimEnd(str);
}

u32 String_ToUpper(MutableString *str)
{
    u32 i;
    for(i = 0; i < str->length; i++)
    {
        str->data[i] = ASCII_ToUpper(str->data[i]);
    }
    return i;
}
u32 String_ToLower(MutableString *str)
{
    u32 i;
    for(i = 0; i < str->length; i++)
    {
        str->data[i] = ASCII_ToLower(str->data[i]);
    }
    return i;
}