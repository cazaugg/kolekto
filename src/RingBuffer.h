// ────────────────────────────────────────────────────────────────────────────
// RingBuffer — fixed-capacity FIFO byte buffer.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// A fixed-capacity FIFO byte buffer. `NEW_RING_BUFFER` embeds the backing
// storage as a compound literal, so the buffer never allocates; the storage
// lives as long as the enclosing scope.
//
// Usage:
//
//   RingBuffer buffer = NEW_RING_BUFFER(64);
//
//   RingBuffer_WriteOne(&buffer, 'a');
//   u8 value = 0;
//   RingBuffer_ReadOne(&buffer, &value);
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_RINGBUFFER
#define KOLEKTO_RINGBUFFER

#include "Datatypes.h"

/** @brief Fixed-capacity FIFO byte buffer over caller-provided storage. */
typedef struct
{
    u8 * const data;
    const Size capacity;
    Size head;
    Size count;
} RingBuffer;

/**
 * @brief Initialize a `RingBuffer` with its own compound-literal storage.
 * @param size Number of bytes the buffer can hold.
 */
#define NEW_RING_BUFFER(size)  (RingBuffer){.data = (u8[size]){0}, .capacity = (size), .head = 0, .count = 0}

/**
 * @brief Number of bytes currently stored.
 * @param buffer Buffer to inspect.
 * @return The number of stored bytes.
 */
Size RingBuffer_Count(RingBuffer const buffer[const static 1]);

/**
 * @brief Maximum number of bytes the buffer can hold.
 * @param buffer Buffer to inspect.
 * @return The buffer capacity.
 */
Size RingBuffer_Capacity(RingBuffer const buffer[const static 1]);

/**
 * @brief Check whether the buffer holds no bytes.
 * @param buffer Buffer to inspect.
 * @return true when empty.
 */
Bool RingBuffer_IsEmpty(RingBuffer const buffer[const static 1]);

/**
 * @brief Check whether the buffer holds no free space.
 * @param buffer Buffer to inspect.
 * @return true when full.
 */
Bool RingBuffer_IsFull(RingBuffer const buffer[const static 1]);

/**
 * @brief Remove all stored bytes.
 * @param buffer Buffer to clear.
 */
void RingBuffer_Clear(RingBuffer buffer[const static 1]);

/**
 * @brief Copy the byte `index` positions after the front, without removing it.
 * @param buffer Buffer to inspect.
 * @param index Offset from the front (0 is the next byte to be read).
 * @param value Receives the inspected byte.
 * @return false when `index` is out of range.
 */
Bool RingBuffer_PeekOne(RingBuffer const buffer[const static 1], Size index, u8 value[const static 1]);

/**
 * @brief Copy up to `length` bytes starting at `index`, without removing them.
 * @param buffer Buffer to inspect.
 * @param index Offset from the front of the first byte to copy.
 * @param length Maximum number of bytes to copy.
 * @param value Receives the copied bytes; `value` must not alias the buffer.
 * @return false when `index` is out of range; otherwise true.
 */
Bool RingBuffer_Peek(RingBuffer const buffer[const static 1], Size index, Size length, u8 value[length]);

/**
 * @brief Remove one byte from the front of the buffer.
 * @param buffer Buffer to read from.
 * @param value Receives the removed byte.
 * @return false when the buffer is empty.
 */
Bool RingBuffer_ReadOne(RingBuffer buffer[const static 1], u8 value[const static 1]);

/**
 * @brief Remove up to `length` bytes into `data`.
 * @param buffer Buffer to read from.
 * @param length Number of bytes to read.
 * @param data Receives the removed bytes.
 * @return The number of bytes actually read.
 */
Size RingBuffer_Read(RingBuffer buffer[const static 1], Size length, u8 data[length]);

/**
 * @brief Remove the single byte at `index`.
 * @param buffer Buffer to remove from.
 * @param index Offset from the front of the byte to remove.
 * @param data Receives the removed byte; `data` must not alias the buffer.
 * @return 1 when a byte was removed, 0 when `index` is out of range.
 */
Size RingBuffer_RemoveOne(RingBuffer buffer[const static 1], Size index, u8 data[const static 1]);

/**
 * @brief Remove up to `length` bytes starting at `index`.
 * @param buffer Buffer to remove from.
 * @param index Offset from the front of the first byte to remove.
 * @param length Maximum number of bytes to remove.
 * @param data Receives the removed bytes; `data` must not alias the buffer.
 * @return The number of bytes actually removed.
 */
Size RingBuffer_Remove(RingBuffer buffer[const static 1], Size index, Size length, u8 data[length]);

/**
 * @brief Append one byte to the back of the buffer.
 * @param buffer Buffer to write into.
 * @param value Byte to store.
 * @return false when the buffer is full.
 */
Bool RingBuffer_WriteOne(RingBuffer buffer[const static 1], u8 value);

/**
 * @brief Append up to `length` bytes from `data`.
 * @param buffer Buffer to write into.
 * @param length Number of bytes to push.
 * @param data Bytes to store.
 * @return The number of bytes actually pushed.
 */
Size RingBuffer_Write(RingBuffer buffer[const static 1], Size length, u8 const data[length]);

/**
 * @brief Insert the single byte `data` at `index`, shifting the tail back.
 * @param buffer Buffer to insert into.
 * @param index Offset from the front where the byte is inserted.
 * @param data Byte to insert; `data` must not alias the buffer.
 * @return 1 when the byte was inserted, 0 when full or `index` is out of range.
 */
Size RingBuffer_InsertOne(RingBuffer buffer[const static 1], Size index, u8 const data[const static 1]);

/**
 * @brief Insert up to `length` bytes at `index`, shifting the tail back.
 * @param buffer Buffer to insert into.
 * @param index Offset from the front where the bytes are inserted.
 * @param length Number of bytes to insert.
 * @param data Bytes to insert; `data` must not alias the buffer.
 * @return The number of bytes actually inserted.
 */
Size RingBuffer_Insert(RingBuffer buffer[const static 1], Size index, Size length, u8 const data[length]);

#endif /* KOLEKTO_RINGBUFFER */
