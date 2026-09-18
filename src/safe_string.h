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
} MutableString;

#define NEW_MUTABLE_STRING(len, init)     (MutableString){.data = (char[len+1]){init}, .capacity = len, .length = strlen(init)}

static inline String StringLiteral_ToSelf(String str)   {return str;}
static inline String MutableString_ToString(MutableString str)   {return str.data;}
#define String_ToString(str)  _Generic((str), String: StringLiteral_ToSelf, char*: StringLiteral_ToSelf, MutableString: MutableString_ToString)(str)

u32 StringLiteral_Length(String str);
u32 MutableString_Length(MutableString str);
#define String_Length(str) _Generic((str), String: StringLiteral_Length, char*: StringLiteral_Length, MutableString: MutableString_Length)(str)

Bool StringLiteral_IsEmpty(String str);
Bool MutableString_IsEmpty(MutableString str);
u32 String_Capacity(MutableString str);
#define String_IsEmpty(str) _Generic((str), String: StringLiteral_IsEmpty, char*: StringLiteral_IsEmpty, MutableString: MutableString_IsEmpty)(str)

Bool StringLiteral_Equal(String a, String b);
#define String_Equal(a, b) StringLiteral_Equal(String_ToString(a), String_ToString(b))

i8 StringLiteral_Compare(String a, String b);
#define String_Compare(a, b) StringLiteral_Compare(String_ToString(a), String_ToString(b))

Bool StringLiteral_StartsWith(String str, String prefix);
#define String_StartsWith(str, prefix) StringLiteral_StartsWith(String_ToString(str), prefix)

Bool StringLiteral_EndsWith(String str, String suffix);
#define String_EndsWith(str, suffix) StringLiteral_EndsWith(String_ToString(str), suffix)

Bool StringLiteral_Contains(String str, String needle);
#define String_Contains(str, needle) StringLiteral_Contains(String_ToString(str), needle)

u32 StringLiteral_Find(String str, String needle);
#define String_Find(str, needle) StringLiteral_Find(String_ToString(str), needle)

u32  StringLiteral_Count(String str, String needle);
#define String_Count(str, needle)   StringLiteral_Count(String_ToString(str), needle)

u32 String_Set(MutableString *str, String value);
void String_Clear(MutableString *str);
Bool String_Append(MutableString *str, String text);
Bool String_AppendChar(MutableString *str, ASCII ch);

u32 String_Join(MutableString *str, String separator, u8 nof_joins, String list[nof_joins]);

u8 String_Split(MutableString *str, String delimiter, u8 nof_splits, String splits[nof_splits]);
u32 String_Insert(MutableString *str, u32 position, String insert);
u32 String_Replace(MutableString *str, String search, String replace);
void String_Reverse(MutableString *str);
u32 String_Trim(MutableString *str);
u32 String_TrimStart(MutableString *str);
u32 String_TrimEnd(MutableString *str);
u32 String_ToUpper(MutableString *str);
u32 String_ToLower(MutableString *str);

#endif /* SAFE_STRINGS */
