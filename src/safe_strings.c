#include "safe_string.h"
#include <string.h>

u32 string_literal_length(string str)
{
    return (u32)strlen(str);
}

u32 string_builder_length(string_builder str)
{
    return str.length;
}

u32 string_builder_capacity(string_builder str)
{
    return str.capacity - str.length;
}

bool string_literal_empty(string str)
{
    return str[0] == '\0';
}

bool string_builder_empty(string_builder str)
{
    return str.length == 0;
}

bool string_literal_equal(string a, string b)
{
    if( a && b) return 0 == strcmp(a, b);
    return false;
}

i8 string_literal_compare(string a, string b)
{
    if(NULL == a) return -1;
    if(NULL == b) return 1;
    return (i8)strcmp(a, b);
}

bool string_literal_starts_with(string str, string prefix)
{
    ASSERT_OR(str && prefix) return false;
    return 0 == strncmp(str, prefix, strlen(prefix));
}

bool string_literal_ends_with(string str, string suffix)
{
    ASSERT_OR(str && suffix) return false;

    u32 suffix_len = string_length(suffix);
    u32 start = string_length(str);
    if(start < suffix_len) return false;
    else start -= suffix_len;
    return 0 == strncmp(str + start, suffix, suffix_len);
}

bool string_literal_contains(string str, string needle)
{
    ASSERT_OR(str && needle) return false;
    ASSERT_OR(needle[0] != '\0') return false;
    return 0 != strstr(str, needle);
}

u32 string_literal_find(string str, string needle)
{
    ASSERT_OR(str && needle) return 0;
    ASSERT_OR(needle[0] != '\0') return 0;

    char const *pos = strstr(str, needle);
    if(NULL == pos) return 0;
    return (u32)(pos - str);
}

u32 string_literal_count(string str, string needle)
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

u32 string_set(string_builder *str, string value)
{
    ASSERT_OR(str && value) return 0;
    u32 value_len = string_length(value);
    ASSERT_OR(value_len < str->capacity) return 0;

    memcpy(str->data, value, value_len);
    str->data[value_len] = '\0';
    str->length = value_len;
    return str->length;
}

void string_clear(string_builder *str)
{
    ASSERT_OR(str) return;
    str->data[0] = '\0';
    str->length = 0;
}

bool string_append(string_builder *str, string text)
{
    ASSERT_OR(str && text) return false;
    u32 value_len = string_length(text);
    ASSERT_OR(str->length + value_len < str->capacity) return false;

    memcpy(str->data + str->length, text, value_len);
    str->length += value_len;
    str->data[str->length] = '\0';
    return true;
}

bool string_append_char(string_builder *str, ascii ch)
{
    ASSERT_OR(str && str->data) return false;
    ASSERT_OR(str->length < str->capacity) return false;

    str->data[str->length] = ch;
    str->length++;
    return true;
}

u32 string_join(string_builder *str, string separator, u8 nof_joins, string list[nof_joins])
{
    ASSERT_OR(str && separator && list) return 0;
    string_set(str, list[0]);

    u32 length = 0;
    length += string_length(list[0]);
    for(u16 i = 1; i < nof_joins; i++)
    {
        ASSERT_OR(list[i]) return 0;
        length += string_length(separator);
        length += string_length(list[i]);
    }

    if(length < str->capacity)
    {
        for(u16 i = 1; i < nof_joins; i++)
        {
            string_append(str, separator);
            string_append(str, list[i]);
        }
        return length;
    }
    return 0;
}

u8 string_split(string_builder *str, string delimiter, u8 nof_splits, string splits[nof_splits])
{
    ASSERT_OR(str && splits) return 0;
    ASSERT_OR(nof_splits > 0) return 0;

    u8 count = 0;
    char *pos = (char *)str->data;

    for(u16 i = 0; i < nof_splits; i++)
    {
        pos += strcspn(pos, delimiter);
        if('\0' != *pos) splits[i] = pos;
        else splits[i] = "";
        pos+= strspn(pos, delimiter);
        count++;
    }
    return count;
}

u32 string_insert(string_builder *str, u32 position, string insert)
{
    ASSERT_OR(str && insert) return 0;
    ASSERT_OR(position <= str->length) return 0;

    u32 insert_len = string_length(insert);
    if(string_builder_capacity(*str) > insert_len)
    {
        u32 shift_len = str->length - position;
        if(shift_len) memmove(&str->data[position + insert_len], &str->data[position], shift_len);
        memcpy(&str->data[position], insert, insert_len);
    }
    return insert_len;
}

u32 string_replace(string_builder *str, string search, string replace)
{
    ASSERT_OR(str &&  replace) return 0;

    u32 spot = string_find(*str, search);
    
    if(string_length(replace) > string_length(search))
    {
        // Expand
        u32 diff = string_length(replace) - string_length(search);
        if(diff > string_builder_capacity(*str)) memmove( &str->data[spot + diff], &str->data[spot], diff);
    }
    else if (string_length(replace) < string_length(search))
    {
        // Shrink
        u32 diff = string_length(search) - string_length(replace);
        if(diff > str->length) memmove( &str->data[spot], &str->data[spot + diff], diff);
    }

    memcpy(&str->data[spot], replace, string_length(replace));
    return string_length(replace);
}

void string_reverse(string_builder *str)
{
    const u32 len = string_length(*str);
    u32 j = len;
    for(u32 i = 0; i < j; i++)
    {
        char temp = str->data[i];
        str->data[i] = str->data[j];
        str->data[j] = temp;
        j++;
    }
}

u32 string_trim_start(string_builder *str)
{
    ASSERT_OR(str) return 0;

    u32 start = 0;
    while(ASCII_IsSpace(str->data[start])) start++;
    str->length -= start;
    memmove(str->data, &str->data[start], str->length);
    str->data[str->length] = '\0';

    return start;
}

u32 string_trim_end(string_builder *str)
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

u32 string_trim(string_builder *str)
{
    return string_trim_start(str) + string_trim_end(str);
}

u32 string_to_upper(string_builder *str)
{
    u32 i;
    for(i = 0; i < string_length(*str); i++)
    {
        ASCII_ToUpper(str->data[i]);
    }
    return i;
}
u32 string_to_lower(string_builder *str)
{
    u32 i;
    for(i = 0; i < string_length(*str); i++)
    {
        ASCII_ToLower(str->data[i]);
    }
    return i;
}