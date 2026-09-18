#ifndef KOLEKTO_BYTEIO
#define KOLEKTO_BYTEIO

#include "datatypes.h"   
#include "byteio.h"

// ────────────────────────────────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────────────────────────────────

typedef struct
{
    ByteIO io;
    String lineEnding;
    Bool echo;
} TextIO;

Size TextIO_Write(TextIO io[const static 1], String text)
{
    return ByteIO_Write(io.io, String_Length(text), (u8*) text);
}

Size TextIO_WriteLine(TextIO io[const static 1], String text)
{
    Size nofBytes = 0;
    nofBytes += ByteIO_Write(io.io, String_Length(text), (u8*) text);
    nofBytes += ByteIO_Write(io.io, String_Length(io.k), (u8*) text);
}

Size TextIO_Read(const TextIO io, Size n, ascii text[n])
{
    Size len = ByteIO_Read(io.io, n, (u8*)text);
    if(io.echo) ByteIO_Write(io.io, n, text);
    return len;
}

Size TextIO_ReadLine(TextIO io, MutableString line)
{
    Size len = String_Length(line);
    bool lineFound = false;
    do
    {
        ascii next;
        bool more = ByteIO_Read(io.io, &next);

        if(more)
        {
            lineFound = String_Find(io.linending, next);
            String_AppendChar(line, next);
            len++;
        }
    } while (more && !lineFound);
    
    return lineFound ? len : 0;
}
