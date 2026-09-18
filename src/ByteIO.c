// ────────────────────────────────────────────────────────────────────────────
// ByteIO — driver-based byte stream abstraction.
// ────────────────────────────────────────────────────────────────────────────

#include "ByteIO.h"

// ── Public functions ─────────────────────────────────────────────────────────

Size ByteIO_Write(ByteIO io[const static 1], Size count, u8 const data[count])
{
    Size i;
    for(i = 0; i < count; i++)
    {
        Bool ok = io->WriteByte(io->driver, data[i]);
        if(ok == false) break;
    }
    return i;
}

Size ByteIO_Read(ByteIO io[const static 1], Size count, u8 data[count])
{
    Size i;
    for(i = 0; i < count; i++)
    {
        Bool ok = io->ReadByte(io->driver, &data[i]);
        if(ok == false) break;
    }
    return i;
}

Bool ByteIO_IsBusy(ByteIO io[const static 1])
{
    IOError ignore;
    return io->GetStatus(io->driver, &ignore);
}

IOError ByteIO_GetError(ByteIO io[const static 1])
{
    IOError error = IOERROR_NONE;
    io->GetStatus(io->driver, &error);
    return error;
}
