#ifndef KOLEKTO_BYTEIO
#define KOLEKTO_BYTEIO

#include "datatypes.h"   

// ────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────
typedef enum 
{
    IO_ERROR_NONE,
    IOERROR_BUFFER_OVERRUN_WRITE,
    IOERROR_BUFFER_OVERRUN_READ,
} IOError;

typedef struct _ByteIO
{
    Any driver;
    bool WriteByte(Any driver, u8 byte);
    bool ReadByte(Any driver, u8* byte);
    bool GetStatus(Any driver, IOError code[const static 1]);
} ByteIO;

Size ByteIO_Write(ByteIO io[const static 1], Size n, u8 data[n])
{
    Size i;
    for(i = 0; i < n; i++)
    {
        bool ok = io->WriteByte(io.driver, data[i]);
        if(ok == false) break;
    }
    return i;
}

Size ByteIO_Read(ByteIO io[const static 1], Size n, u8 data[n])
{
    Size i;
    for(i = 0; i < n; i++)
    {
        bool ok = io->ReadByte(io.driver, &data[i])
        if(ok == false) break;
    }
    return i;
}

bool BytIO_IsBusy(ByteIO io[const static 1])
{
    IOError ignore;
    return io->GetStatus(io->driver, &ignore);
}

IOError ByteIO_GetError(ByteIO io[const static 1])
{
    IOError err;
    return io->GetStatus(io->driver, &err);

}

#endif