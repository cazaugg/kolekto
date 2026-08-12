#include "datatypes.h"
#include "ascii.h"
#include <string.h>

#ifndef SAFE_STRINGS
#define SAFE_STRINGS

typedef struct
{
    char * const data;
    const u32 capacity;
    u32 length;
} string_builder;

#define NEW_STRING_BUILDER(len, init)     (string_builder){.data = (char[len+1]){init}, .capacity = len, .length = strlen(init)}

static inline string string_literal_to_self(string str)   {return str;}
static inline string string_builder_to_literal(string_builder str)   {return str.data;}
#define to_string(str)  _Generic((str), string: string_literal_to_self, char*: string_literal_to_self, string_builder: string_builder_to_literal)(str)

u32 string_literal_length(string str);
u32 string_builder_length(string_builder str);
#define string_length(str) _Generic((str), string: string_literal_length, string_builder: string_builder_length)(str)

bool string_literal_empty(string str);
bool string_builder_empty(string_builder str);
u32 string_builder_capacity(string_builder str);
#define string_empty(str) _Generic((str), string: string_literal_empty, string_builder: string_builder_empty)(str)

bool string_literal_equal(string a, string b);
#define string_equal(a, b) string_literal_equal(to_string(a), to_string(b))

i8 string_literal_compare(string a, string b);
#define string_compare(a, b) string_literal_compare(to_string(a), to_string(b))

bool string_literal_starts_with(string str, string prefix);  
#define string_starts_with(str, prefix) string_literal_starts_with(to_string(str), prefix)  

bool string_literal_ends_with(string str, string suffix);
#define string_ends_with(str, suffix) string_literal_ends_with(to_string(str), suffix)

bool string_literal_contains(string str, string needle);
#define string_contains(str, needle) string_literal_contains(to_string(str), needle)

u32 string_literal_find(string str, string needle);
#define string_find(str, needle) string_literal_find(to_string(str), needle)

u32  string_literal_count(string str, string needle); 
#define string_count(str, needle)   string_literal_count(to_string(str), needle)

u32 string_set(string_builder *str, string value);
void string_clear(string_builder *str);
bool string_append(string_builder *str, string text);
bool string_append_char(string_builder *str, ascii ch);

u32 string_join(string_builder *str, string separator, u8 nof_joins, string list[nof_joins]);

u8 string_split(string_builder *str, string delimiter, u8 nof_splits, string splits[nof_splits]);
u32 string_insert(string_builder *str, u32 position, string insert);
u32 string_replace(string_builder *str, string search, string replace);
void string_reverse(string_builder *str);
u32 string_trim(string_builder *str);
u32 string_trim_start(string_builder *str);
u32 string_trim_end(string_builder *str);
u32 string_to_upper(string_builder *str);
u32 string_to_lower(string_builder *str);

#endif /* SAFE_STRINGS */