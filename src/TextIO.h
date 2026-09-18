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
    String line_ending;
    Bool echo;
} TextIO;

/**
 * @brief Write `text` to the stream.
 * @return The number of bytes written.
 */
static inline Size TextIO_Write(TextIO io[const static 1], String text)
{
    return ByteIO_Write(&io->io, String_Length(text), (u8*) text);
}

/**
 * @brief Write `text` followed by the stream's line ending.
 * @return The number of bytes written.
 */
static inline Size TextIO_WriteLine(TextIO io[const static 1], String text)
{
    Size number_of_bytes = 0;
    number_of_bytes += ByteIO_Write(&io->io, String_Length(text), (u8*) text);
    number_of_bytes += ByteIO_Write(&io->io, String_Length(io->line_ending), (u8*) io->line_ending);
    return number_of_bytes;
}

/**
 * @brief Read up to `count` bytes into `text`, echoing when enabled.
 * @return The number of bytes read.
 */
static inline Size TextIO_Read(TextIO io[const static 1], Size count, ASCII text[count])
{
    Size length = ByteIO_Read(&io->io, count, (u8*) text);
    if(io->echo) ByteIO_Write(&io->io, count, (u8*) text);
    return length;
}

/**
 * @brief Read one line into `line` (line ending included).
 * @return The line length, or 0 if the stream ended first.
 */
static inline Size TextIO_ReadLine(TextIO io[const static 1], MutableString *line)
{
    Size length = 0;
    Bool line_found = false;
    do
    {
        ASCII next = '\0';
        Bool more = ByteIO_Read(&io->io, 1, (u8*) &next);
        if(!more) break;

        if(!String_AppendChar(line, next)) break;
        length++;

        line_found = String_EndsWith(line->data, io->line_ending);
    } while(!line_found);

    return line_found ? length : 0;
}

#endif /* KOLEKTO_TEXTIO */
