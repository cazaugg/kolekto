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
