#ifndef KOLEKTO_ASCII
#define KOLEKTO_ASCII

#include "datatypes.h"

/** Portable representation of a single ASCII character (0-127). */
typedef unsigned char ascii;

/**
 * @brief Check if a value is a valid ASCII character.
 * @param character Value to check.
 * @return true if character is in range [0, 127].
 */
bool ASCII_IsValid(ascii character);

/**
 * @brief Check if character is alphanumeric (letter or digit).
 * @param character ASCII character to check.
 * @return true if character is in [A-Za-z0-9].
 */
bool ASCII_IsAlphaNumeric(ascii character);

/**
 * @brief Check if character is a letter (upper or lower case).
 * @param character ASCII character to check.
 * @return true if character is in [A-Za-z].
 */
bool ASCII_IsLetter(ascii character);

/**
 * @brief Check if character is a lower-case letter.
 * @param character ASCII character to check.
 * @return true if character is in [a-z].
 */
bool ASCII_IsLowerCase(ascii character);

/**
 * @brief Check if character is an upper-case letter.
 * @param character ASCII character to check.
 * @return true if character is in [A-Z].
 */
bool ASCII_IsUpperCase(ascii character);

/**
 * @brief Check if character is a decimal digit.
 * @param character ASCII character to check.
 * @return true if character is in [0-9].
 */
bool ASCII_IsDigit(ascii character);

/**
 * @brief Check if character is a hexadecimal digit.
 * @param character ASCII character to check.
 * @return true if character is in [0-9A-Fa-f].
 */
bool ASCII_IsHexDigit(ascii character);

/**
 * @brief Check if character is printable (letter, digit, punctuation, or space).
 * @param character ASCII character to check.
 * @return true if character has a visible glyph or is space (0x20-0x7E plus space).
 */
bool ASCII_IsPrintable(ascii character);

/**
 * @brief Check if character is whitespace.
 * @param character ASCII character to check.
 * @return true if character is space, tab, newline, vtab, formfeed, or carriage return.
 */
bool ASCII_IsSpace(ascii character);

/**
 * @brief Check if character is punctuation.
 * @param character ASCII character to check.
 * @return true if character is a printable non-alphanumeric, non-space character.
 */
bool ASCII_IsPunctuation(ascii character);

/**
 * @brief Check if character is a control code.
 * @param character ASCII character to check.
 * @return true if character is in [0x00-0x1F] or 0x7F (DEL).
 */
bool ASCII_IsControlCode(ascii character);

/**
 * @brief Convert an upper-case letter to lower case.
 * @param character ASCII character to convert.
 * @return The lower-case equivalent if character is [A-Z]; unchanged otherwise.
 */
ascii ASCII_ToLower(ascii character);

/**
 * @brief Convert a lower-case letter to upper case.
 * @param character ASCII character to convert.
 * @return The upper-case equivalent if character is [a-z]; unchanged otherwise.
 */
ascii ASCII_ToUpper(ascii character);

#endif /* KOLEKTO_ASCII */
