// ────────────────────────────────────────────────────────────────────────────
// SafeStrings — capacity-checked string and mutable string operations.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// Capacity-checked helpers for C strings (`String`/`char*`) and a
// fixed-capacity `MutableString` buffer. Nothing allocates: operations that
// would exceed a buffer's capacity fail with `false` or `0`.
//
// Usage:
//
//   MutableString text = NEW_MUTABLE_STRING(32, "");
//   String_Append(&text, "hello ");
//   String_Append(&text, "world");
//
//   String literal = "value=42";
//   if(String_StartsWith(literal, "value")) { /* ... */ }
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_SAFESTRINGS
#define KOLEKTO_SAFESTRINGS

#include "Datatypes.h"
#include "ASCII.h"
#include <string.h>

/** @brief Growable, fixed-capacity UTF-8/ASCII string buffer. */
typedef struct
{
    char * const data;
    const u32 capacity;
    u32 length;
} MutableString;

/**
 * @brief Statically initialize a `MutableString` with inline storage.
 * @param size Usable capacity in characters.
 * @param initial Initial contents.
 */
#define NEW_MUTABLE_STRING(size, initial)     (MutableString){.data = (char[size+1]){initial}, .capacity = size, .length = strlen(initial)}

/**
 * @brief Statically initialize an empty `MutableString` with inline storage.
 * @param size Usable capacity in characters.
 */
#define NEW_MUTABLE_STRING_EMPTY(size)     (MutableString){.data = (char[size+1]){0}, .capacity = size, .length = 0}

/** @brief Identity conversion used by `String_ToString` for string literals. */
static inline String StringLiteral_ToSelf(String string)   {return string;}
/** @brief Convert a `MutableString` to its underlying `String`. */
static inline String MutableString_ToString(MutableString string)   {return string.data;}

/** @brief Convert a `String`, `char*` or `MutableString` to a `String`. */
#define String_ToString(string)  _Generic((string), String: StringLiteral_ToSelf, char*: StringLiteral_ToSelf, MutableString: MutableString_ToString)(string)

/** @brief Length of a null-terminated string. */
u32 StringLiteral_Length(String string);
/** @brief Current length of a `MutableString`. */
u32 MutableString_Length(MutableString string);
/** @brief Generic length of a `String`, `char*` or `MutableString`. */
#define String_Length(string) _Generic((string), String: StringLiteral_Length, char*: StringLiteral_Length, MutableString: MutableString_Length)(string)

/** @brief True if a null-terminated string is empty. */
Bool StringLiteral_IsEmpty(String string);
/** @brief True if a `MutableString` is empty. */
Bool MutableString_IsEmpty(MutableString string);
/** @brief Free capacity remaining in a `MutableString`. */
u32 String_Capacity(MutableString string);
/** @brief Generic emptiness test for `String`, `char*` or `MutableString`. */
#define String_IsEmpty(string) _Generic((string), String: StringLiteral_IsEmpty, char*: StringLiteral_IsEmpty, MutableString: MutableString_IsEmpty)(string)

/** @brief Compare two strings for equality. */
Bool StringLiteral_Equal(String a, String b);
/** @brief Generic equality test for `String`, `char*` or `MutableString`. */
#define String_Equal(a, b) StringLiteral_Equal(String_ToString(a), String_ToString(b))

/** @brief Compare two strings (like `strcmp`, returns i8). */
i8 StringLiteral_Compare(String a, String b);
/** @brief Generic lexicographic comparison. */
#define String_Compare(a, b) StringLiteral_Compare(String_ToString(a), String_ToString(b))

/** @brief True if `string` begins with `prefix`. */
Bool StringLiteral_StartsWith(String string, String prefix);
/** @brief Generic prefix test. */
#define String_StartsWith(string, prefix) StringLiteral_StartsWith(String_ToString(string), prefix)

/** @brief True if `string` ends with `suffix`. */
Bool StringLiteral_EndsWith(String string, String suffix);
/** @brief Generic suffix test. */
#define String_EndsWith(string, suffix) StringLiteral_EndsWith(String_ToString(string), suffix)

/** @brief True if `needle` occurs in `string`. */
Bool StringLiteral_Contains(String string, String needle);
/** @brief Generic substring test. */
#define String_Contains(string, needle) StringLiteral_Contains(String_ToString(string), needle)

/** @brief Index of the first occurrence of `needle`, or 0 if absent. */
u32 StringLiteral_Find(String string, String needle);
/** @brief Generic substring search. */
#define String_Find(string, needle) StringLiteral_Find(String_ToString(string), needle)

/** @brief Number of non-overlapping occurrences of `needle`. */
u32  StringLiteral_Count(String string, String needle);
/** @brief Generic substring count. */
#define String_Count(string, needle)   StringLiteral_Count(String_ToString(string), needle)

/** @brief Overwrite a `MutableString`; returns the new length (0 on failure). */
u32 String_Set(MutableString *string, String value);
/** @brief Set a `MutableString` to the empty string. */
void String_Clear(MutableString *string);
/** @brief Append `text`; returns false when capacity is exceeded. */
Bool String_Append(MutableString *string, String text);
/** @brief Append a single character; returns false when full. */
Bool String_AppendChar(MutableString *string, ASCII character);

/** @brief Join `number_of_joins` strings with `separator`; returns length or 0. */
u32 String_Join(MutableString *string, String separator, u8 number_of_joins, String list[number_of_joins]);

/** @brief Split on any delimiter character; returns the number of parts. */
u8 String_Split(MutableString *string, String delimiter, u8 number_of_splits, String splits[number_of_splits]);
/** @brief Insert `insert` at `position`; returns the number of characters added. */
u32 String_Insert(MutableString *string, u32 position, String insert);
/** @brief Replace the first occurrence of `search` with `replace`. */
u32 String_Replace(MutableString *string, String search, String replace);
/** @brief Reverse a `MutableString` in place. */
void String_Reverse(MutableString *string);
/** @brief Trim leading and trailing whitespace; returns the number removed. */
u32 String_Trim(MutableString *string);
/** @brief Trim leading whitespace; returns the number removed. */
u32 String_TrimStart(MutableString *string);
/** @brief Trim trailing whitespace; returns the number removed. */
u32 String_TrimEnd(MutableString *string);
/** @brief Upper-case a `MutableString` in place; returns its length. */
u32 String_ToUpper(MutableString *string);
/** @brief Lower-case a `MutableString` in place; returns its length. */
u32 String_ToLower(MutableString *string);

#endif /* KOLEKTO_SAFESTRINGS */
