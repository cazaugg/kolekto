// ────────────────────────────────────────────────────────────────────────────
// TextIO — line-oriented text streams built on ByteIO.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// Adds line-oriented text handling on top of a `ByteIO`: it knows the line
// ending to write and reads incoming bytes into its internal `line_buffer`
// until that ending is seen.
//
// Usage:
//
//   TextIO console = NEW_TEXT_IO(io, 128, "\n", ECHO_ON);
//
//   TextIO_WriteLine(&console, "hello");
//
//   Size length = TextIO_ReadLine(&console);
//   MutableString line = TextIO_GetLine(&console);
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_TEXTIO
#define KOLEKTO_TEXTIO

#include "Datatypes.h"
#include "ByteIO.h"
#include "SafeStrings.h"

// ────────────────────────────────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────────────────────────────────

/** @brief Text stream layered on top of a `ByteIO` driver. */
typedef struct
{
    ByteIO io;
    MutableString line_buffer;
    String line_ending;
    Bool echo;
} TextIO;

/**
 * @brief Statically initialize a `TextIO`.
 * @param io_value Underlying `ByteIO`.
 * @param max_line_length Capacity of the internal line buffer.
 * @param ending Line ending written by `TextIO_WriteLine`.
 * @param echo_on Whether `TextIO_Read` echoes what it read (see `ECHO_ON`).
 */
#define NEW_TEXT_IO(io_value, max_line_length, ending, echo_on) \
    (TextIO){.io = (io_value), \
             .line_buffer = NEW_MUTABLE_STRING_EMPTY(max_line_length), \
             .line_ending = (ending), \
             .echo = (echo_on)}

#define ECHO_ON  true
#define ECHO_OFF false

/**
 * @brief Write `text` to the stream.
 * @param io Stream to write to.
 * @param text Text to write.
 * @return The number of bytes written.
 */
Size TextIO_Write(TextIO io[const static 1], String text);

/**
 * @brief Write `text` followed by the stream's line ending.
 * @param io Stream to write to.
 * @param text Text to write.
 * @return The number of bytes written.
 */
Size TextIO_WriteLine(TextIO io[const static 1], String text);

/**
 * @brief Read up to `count` bytes into `text`, echoing when enabled.
 * @param io Stream to read from.
 * @param count Maximum number of bytes to read.
 * @param text Receives the read bytes.
 * @return The number of bytes actually read.
 */
Size TextIO_Read(TextIO io[const static 1], Size count, ASCII text[count]);

/**
 * @brief Read one line into the internal `line_buffer`.
 * @param io Stream to read from.
 * @return The line length when a line ending was found, otherwise 0.
 */
Size TextIO_ReadLine(TextIO io[const static 1]);

/**
 * @brief Return the stream's internal line buffer.
 * @param io Stream to inspect.
 * @return A copy of the last line read by `TextIO_ReadLine`.
 */
MutableString TextIO_GetLine(TextIO const io[const static 1]);

#endif /* KOLEKTO_TEXTIO */
