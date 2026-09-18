// ────────────────────────────────────────────────────────────────────────────
// RingBuffer — fixed-capacity FIFO byte buffer.
// ────────────────────────────────────────────────────────────────────────────

#include "RingBuffer.h"

// ── Internal functions ───────────────────────────────────────────────────────

/**
 * @brief Wrap an absolute storage index into `[0, capacity)`.
 * @param buffer Buffer whose capacity is used.
 * @param index Absolute index (may exceed the capacity).
 * @return The wrapped index.
 */
static Size WrapIndex(RingBuffer const buffer[const static 1], Size index);

/**
 * @brief Move the logical range `[index, count)` up by `length` slots.
 * @param buffer Buffer to shift within.
 * @param index First logical index to move.
 * @param length Number of slots to shift by.
 */
static void ShiftUp(RingBuffer buffer[const static 1], Size index, Size length);

/**
 * @brief Move the logical range `[index + length, count)` down by `length`.
 * @param buffer Buffer to shift within.
 * @param index First logical index of the vacated gap.
 * @param length Number of slots to shift by.
 */
static void ShiftDown(RingBuffer buffer[const static 1], Size index, Size length);

// ── Public functions ─────────────────────────────────────────────────────────

Size RingBuffer_Count(RingBuffer const buffer[const static 1])
{
    return buffer->count;
}

Size RingBuffer_Capacity(RingBuffer const buffer[const static 1])
{
    return buffer->capacity;
}

Bool RingBuffer_IsEmpty(RingBuffer const buffer[const static 1])
{
    return buffer->count == 0;
}

Bool RingBuffer_IsFull(RingBuffer const buffer[const static 1])
{
    return buffer->count == buffer->capacity;
}

void RingBuffer_Clear(RingBuffer buffer[const static 1])
{
    buffer->head = 0;
    buffer->count = 0;
}

Bool RingBuffer_PeekOne(RingBuffer const buffer[const static 1], Size index, u8 value[const static 1])
{
    if(index >= buffer->count) return false;

    *value = buffer->data[WrapIndex(buffer, buffer->head + index)];
    return true;
}

Bool RingBuffer_Peek(RingBuffer const buffer[const static 1], Size index, Size length, u8 value[length])
{
    if(index >= buffer->count) return false;

    Size available = buffer->count - index;
    Size copied = (length < available) ? length : available;
    for(Size i = 0; i < copied; i++)
    {
        value[i] = buffer->data[WrapIndex(buffer, buffer->head + index + i)];
    }
    return true;
}

Bool RingBuffer_ReadOne(RingBuffer buffer[const static 1], u8 value[const static 1])
{
    if(RingBuffer_IsEmpty(buffer)) return false;

    *value = buffer->data[buffer->head];
    buffer->head = WrapIndex(buffer, buffer->head + 1);
    buffer->count--;
    return true;
}

Size RingBuffer_Read(RingBuffer buffer[const static 1], Size length, u8 data[length])
{
    Size read = 0;
    while(read < length && !RingBuffer_IsEmpty(buffer))
    {
        RingBuffer_ReadOne(buffer, &data[read]);
        read++;
    }
    return read;
}

Size RingBuffer_RemoveOne(RingBuffer buffer[const static 1], Size index, u8 data[const static 1])
{
    if(index >= buffer->count) return 0;

    *data = buffer->data[WrapIndex(buffer, buffer->head + index)];
    ShiftDown(buffer, index, 1);
    buffer->count--;
    return 1;
}

Size RingBuffer_Remove(RingBuffer buffer[const static 1], Size index, Size length, u8 data[length])
{
    if(index >= buffer->count) return 0;

    Size available = buffer->count - index;
    Size removed = (length < available) ? length : available;
    for(Size i = 0; i < removed; i++)
    {
        data[i] = buffer->data[WrapIndex(buffer, buffer->head + index + i)];
    }
    ShiftDown(buffer, index, removed);
    buffer->count -= removed;
    return removed;
}

Bool RingBuffer_WriteOne(RingBuffer buffer[const static 1], u8 value)
{
    if(RingBuffer_IsFull(buffer)) return false;

    buffer->data[WrapIndex(buffer, buffer->head + buffer->count)] = value;
    buffer->count++;
    return true;
}

Size RingBuffer_Write(RingBuffer buffer[const static 1], Size length, u8 const data[length])
{
    Size written = 0;
    while(written < length && !RingBuffer_IsFull(buffer))
    {
        RingBuffer_WriteOne(buffer, data[written]);
        written++;
    }
    return written;
}

Size RingBuffer_InsertOne(RingBuffer buffer[const static 1], Size index, u8 const data[const static 1])
{
    if(index > buffer->count || RingBuffer_IsFull(buffer)) return 0;

    ShiftUp(buffer, index, 1);
    buffer->data[WrapIndex(buffer, buffer->head + index)] = *data;
    buffer->count++;
    return 1;
}

Size RingBuffer_Insert(RingBuffer buffer[const static 1], Size index, Size length, u8 const data[length])
{
    if(index > buffer->count) return 0;

    Size free_space = buffer->capacity - buffer->count;
    Size inserted = (length < free_space) ? length : free_space;
    if(inserted == 0) return 0;

    ShiftUp(buffer, index, inserted);
    for(Size i = 0; i < inserted; i++)
    {
        buffer->data[WrapIndex(buffer, buffer->head + index + i)] = data[i];
    }
    buffer->count += inserted;
    return inserted;
}

// ── Internal implementation ──────────────────────────────────────────────────

static Size WrapIndex(RingBuffer const buffer[const static 1], Size index)
{
    return index % buffer->capacity;
}

static void ShiftUp(RingBuffer buffer[const static 1], Size index, Size length)
{
    Size i = buffer->count;
    while(i > index)
    {
        i--;
        buffer->data[WrapIndex(buffer, buffer->head + i + length)] =
            buffer->data[WrapIndex(buffer, buffer->head + i)];
    }
}

static void ShiftDown(RingBuffer buffer[const static 1], Size index, Size length)
{
    for(Size i = index + length; i < buffer->count; i++)
    {
        buffer->data[WrapIndex(buffer, buffer->head + i - length)] =
            buffer->data[WrapIndex(buffer, buffer->head + i)];
    }
}
