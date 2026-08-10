#include "datatypes.h"

static inline string __sl_return_self(string str)   {return str;}
static inline string __sb_return_sl(string_builder str)   {return str.data;}

#define to_string(str)  _Generic((str), string: __string_return_self, string_builder: __string_return_string)

#define string_length(str) _Generic((str), string: __sl_length, string_builder: __sb_length)(str)
#define string_empty(str) _Generic((str), string: __sl_empty, string_builder: __sb_empty)(str)
#define string_compare(a, b) __sl_compare(to_string(a), to_string(b))
#define string_equal(a, b) __sl_equal(to_string(a), to_string(b))

u32 __sl_length(string str);
u32 __sb_length(string str);

bool __sl_empty(string str);
bool __sb_empty(string_builder str);

bool __sl_equal(string a, string b);            
i8 __sl_compare(string a, string b);

bool string_starts_with(string str, string prefix);  
bool string_ends_with(string str, string suffix);
bool string_contains(string str, string needle);     
u32  string_find(string str, string needle);
u32  string_count(string str, string needle); 

u32 string_set(string_builder string, string value);
void string_clear(string_builder str);
bool string_append(string_builder str, string text);
bool string_append_char(string_builder str, ascii ch);

bool string_join(string_builder str, string separator, u8 nof_joins, string list[nof_joins]);
u8 string_split(string_builder str, u8 nof_splits, string splits[nof_splits]);
u32 string_insert(string_builder str, u32 position, string insert);
u32 string_replace(string_builder str, string search, string replace);
void string_reverse(string_builder str);

u32 string_trim(string_builder str);
u32 string_trim_start(string_builder str);
u32 string_trim_end(string_builder str);

u32 string_to_upper(string_builder str);
u32 string_to_lower(string_builder str);