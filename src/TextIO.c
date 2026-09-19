// ────────────────────────────────────────────────────────────────────────────
// TextIO — line-oriented text streams built on ByteIO.
// ────────────────────────────────────────────────────────────────────────────

#include "TextIO.h"

#include <stdarg.h>
#include <stdio.h>

// ── Internal functions ───────────────────────────────────────────────────────

/**
 * @brief Format text into a fixed buffer and write it to the stream.
 * @param io Stream to write to.
 * @param format printf-style format string.
 * @param args Format arguments.
 * @return The number of bytes written.
 */
static Size PrintFormatted(TextIO io[const static 1], String format, va_list args);

// ── Public functions ─────────────────────────────────────────────────────────

Size TextIO_Write(TextIO io[const static 1], String text)
{
    return ByteIO_Write(&io->io, String_Length(text), (u8*) text);
}

Size TextIO_WriteLine(TextIO io[const static 1], String text)
{
    Size number_of_bytes = 0;
    number_of_bytes += TextIO_Write(io, text);
    number_of_bytes += TextIO_Write(io, io->line_ending);
    return number_of_bytes;
}

Size TextIO_Read(TextIO io[const static 1], Size count, ASCII text[count])
{
    Size length = ByteIO_Read(&io->io, count, (u8*) text);
    for(Size i = 0; i < length; i++)
    {
        String_AppendChar(&io->line_buffer, text[i]);
    }
    if(io->echo) ByteIO_Write(&io->io, length, (u8*) text);
    return length;
}

Size TextIO_ReadLine(TextIO io[const static 1])
{
    String_Clear(&io->line_buffer);

    Size length = 0;
    Bool line_found = false;
    while(!line_found)
    {
        ASCII next = '\0';
        if(!ByteIO_Read(&io->io, 1, (u8*) &next)) break;
        if(!String_AppendChar(&io->line_buffer, next)) break;

        length++;
        line_found = String_EndsWith(io->line_buffer, io->line_ending);
    }
    return line_found ? length : 0;
}

MutableString TextIO_GetLine(TextIO const io[const static 1])
{
    return io->line_buffer;
}

void TextIO_LineBreak(TextIO io[const static 1])
{
    TextIO_Write(io, io->line_ending);
}

Size TextIO_Print(TextIO io[const static 1], String format, ...)
{
    va_list args;
    va_start(args, format);
    Size written = PrintFormatted(io, format, args);
    va_end(args);
    return written;
}

Size TextIO_PrintLine(TextIO io[const static 1], String format, ...)
{
    va_list args;
    va_start(args, format);
    Size written = PrintFormatted(io, format, args);
    va_end(args);
    TextIO_LineBreak(io);
    return written + String_Length(io->line_ending);
}

// ── Internal implementation ──────────────────────────────────────────────────

static Size PrintFormatted(TextIO io[const static 1], String format, va_list args)
{
    char buffer[256];

    int written = vsnprintf(buffer, sizeof(buffer), format, args);
    if(written < 0) return 0;

    return TextIO_Write(io, buffer);
}
