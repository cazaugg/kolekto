#ifndef KOLEKTO_BYTEIO
#define KOLEKTO_BYTEIO

#include "Datatypes.h"

// ────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────

/** @brief Error codes reported by a `ByteIO` driver. */
typedef enum
{
    IOERROR_NONE,
    IOERROR_BUFFER_OVERRUN_WRITE,
    IOERROR_BUFFER_OVERRUN_READ,
} IOError;

/**
 * @brief Driver-based byte stream.
 *
 * The function-pointer members follow the function naming conventions and
 * define the driver interface implemented by each backend.
 */
typedef struct
{
    Any driver;
    Bool (*WriteByte)(Any driver, u8 byte);
    Bool (*ReadByte)(Any driver, u8* byte);
    Bool (*GetStatus)(Any driver, IOError code[const static 1]);
} ByteIO;

/**
 * @brief Write `count` bytes from `data`.
 * @return The number of bytes actually written.
 */
static inline Size ByteIO_Write(ByteIO io[const static 1], Size count, u8 data[count])
{
    Size i;
    for(i = 0; i < count; i++)
    {
        Bool ok = io->WriteByte(io->driver, data[i]);
        if(ok == false) break;
    }
    return i;
}

/**
 * @brief Read up to `count` bytes into `data`.
 * @return The number of bytes actually read.
 */
static inline Size ByteIO_Read(ByteIO io[const static 1], Size count, u8 data[count])
{
    Size i;
    for(i = 0; i < count; i++)
    {
        Bool ok = io->ReadByte(io->driver, &data[i]);
        if(ok == false) break;
    }
    return i;
}

/** @brief True while the driver reports that it cannot accept more bytes. */
static inline Bool ByteIO_IsBusy(ByteIO io[const static 1])
{
    IOError ignore;
    return io->GetStatus(io->driver, &ignore);
}

/** @brief Return the last error reported by the driver. */
static inline IOError ByteIO_GetError(ByteIO io[const static 1])
{
    IOError error = IOERROR_NONE;
    io->GetStatus(io->driver, &error);
    return error;
}

#endif /* KOLEKTO_BYTEIO */
