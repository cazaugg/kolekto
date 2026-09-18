// ────────────────────────────────────────────────────────────────────────────
// TextIO — line-oriented text streams built on ByteIO.
// ────────────────────────────────────────────────────────────────────────────

#include "TextIO.h"

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
