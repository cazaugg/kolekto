// ────────────────────────────────────────────────────────────────────────────
// ByteIO — driver-based byte stream abstraction.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// A `ByteIO` couples an opaque `driver` with the callbacks `WriteByte`,
// `ReadByte` and `GetStatus`. Driver state and behaviour live behind those
// function pointers, so the same helpers serve memory, files, serial links or
// test doubles.
//
// Usage:
//
//   ByteIO io = NEW_BYTE_IO(&device, Device_WriteByte, Device_ReadByte,
//                           Device_GetStatus);
//
//   u8 data[4] = {0};
//   Size count = ByteIO_Read(&io, 4, data);
// ────────────────────────────────────────────────────────────────────────────

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
 * @brief Statically initialize a `ByteIO` from a driver and its callbacks.
 * @param device Opaque driver state passed to every callback.
 * @param write_byte Write callback.
 * @param read_byte Read callback.
 * @param get_status Status callback (returns busy, stores the current error).
 */
#define NEW_BYTE_IO(device, write_byte, read_byte, get_status) \
    (ByteIO){.driver = (device), \
             .WriteByte = (write_byte), \
             .ReadByte = (read_byte), \
             .GetStatus = (get_status)}

/**
 * @brief Write `count` bytes from `data`.
 * @param io Stream to write to.
 * @param count Number of bytes to write.
 * @param data Bytes to write.
 * @return The number of bytes actually written.
 */
Size ByteIO_Write(ByteIO io[const static 1], Size count, u8 const data[count]);

/**
 * @brief Read up to `count` bytes into `data`.
 * @param io Stream to read from.
 * @param count Maximum number of bytes to read.
 * @param data Receives the read bytes.
 * @return The number of bytes actually read.
 */
Size ByteIO_Read(ByteIO io[const static 1], Size count, u8 data[count]);

/**
 * @brief Query whether the driver cannot accept more bytes.
 * @param io Stream to inspect.
 * @return true when the driver reports itself busy.
 */
Bool ByteIO_IsBusy(ByteIO io[const static 1]);

/**
 * @brief Return the last error reported by the driver.
 * @param io Stream to inspect.
 * @return The current `IOError` code.
 */
IOError ByteIO_GetError(ByteIO io[const static 1]);

#endif /* KOLEKTO_BYTEIO */
