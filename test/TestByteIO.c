// ────────────────────────────────────────────────────────────────────────────
// TestByteIO — Unity tests for the ByteIO module.
// ────────────────────────────────────────────────────────────────────────────

#include "unity.h"
#include "../src/ByteIO.h"

#include <string.h>

typedef struct
{
    u8 output[64];
    Size output_count;
    Size output_limit;
    u8 input[64];
    Size input_count;
    Size input_position;
    Bool busy;
    IOError error;
} MockDriver;

static Bool MockWriteByte(Any driver, u8 byte)
{
    MockDriver *mock = (MockDriver*) driver;
    if(mock->output_count >= mock->output_limit) return false;

    mock->output[mock->output_count] = byte;
    mock->output_count++;
    return true;
}

static Bool MockReadByte(Any driver, u8 *byte)
{
    MockDriver *mock = (MockDriver*) driver;
    if(mock->input_position >= mock->input_count) return false;

    *byte = mock->input[mock->input_position];
    mock->input_position++;
    return true;
}

static Bool MockGetStatus(Any driver, IOError code[const static 1])
{
    MockDriver *mock = (MockDriver*) driver;
    *code = mock->error;
    return mock->busy;
}

static ByteIO MakeStream(MockDriver *mock)
{
    return NEW_BYTE_IO(mock, MockWriteByte, MockReadByte, MockGetStatus);
}

void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_ByteIO_Write_Valid(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO io = MakeStream(&mock);
    const u8 data[3] = {1, 2, 3};

    TEST_ASSERT_EQUAL_size_t(3, ByteIO_Write(&io, 3, data));
    TEST_ASSERT_EQUAL_size_t(3, mock.output_count);
    TEST_ASSERT_EQUAL_UINT8(1, mock.output[0]);
    TEST_ASSERT_EQUAL_UINT8(2, mock.output[1]);
    TEST_ASSERT_EQUAL_UINT8(3, mock.output[2]);
}

static void Test_ByteIO_Write_Partial(void)
{
    MockDriver mock = {0};
    mock.output_limit = 2;
    ByteIO io = MakeStream(&mock);
    const u8 data[4] = {1, 2, 3, 4};

    TEST_ASSERT_EQUAL_size_t(2, ByteIO_Write(&io, 4, data));
    TEST_ASSERT_EQUAL_size_t(2, mock.output_count);
}

static void Test_ByteIO_Write_Empty(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO io = MakeStream(&mock);
    const u8 data[1] = {1};

    TEST_ASSERT_EQUAL_size_t(0, ByteIO_Write(&io, 0, data));
    TEST_ASSERT_EQUAL_size_t(0, mock.output_count);
}

static void Test_ByteIO_Read_Valid(void)
{
    MockDriver mock = {0};
    ByteIO io = MakeStream(&mock);
    u8 data[3] = {0};

    memcpy(mock.input, "\x04\x05\x06", 3);
    mock.input_count = 3;

    TEST_ASSERT_EQUAL_size_t(3, ByteIO_Read(&io, 3, data));
    TEST_ASSERT_EQUAL_UINT8(4, data[0]);
    TEST_ASSERT_EQUAL_UINT8(5, data[1]);
    TEST_ASSERT_EQUAL_UINT8(6, data[2]);
}

static void Test_ByteIO_Read_Eof(void)
{
    MockDriver mock = {0};
    ByteIO io = MakeStream(&mock);
    u8 data[5] = {0};

    memcpy(mock.input, "\x01\x02", 2);
    mock.input_count = 2;

    TEST_ASSERT_EQUAL_size_t(2, ByteIO_Read(&io, 5, data));
    TEST_ASSERT_EQUAL_UINT8(1, data[0]);
    TEST_ASSERT_EQUAL_UINT8(2, data[1]);
}

static void Test_ByteIO_IsBusy_True(void)
{
    MockDriver mock = {0};
    mock.busy = true;
    ByteIO io = MakeStream(&mock);

    TEST_ASSERT_TRUE(ByteIO_IsBusy(&io));
}

static void Test_ByteIO_IsBusy_False(void)
{
    MockDriver mock = {0};
    mock.busy = false;
    ByteIO io = MakeStream(&mock);

    TEST_ASSERT_FALSE(ByteIO_IsBusy(&io));
}

static void Test_ByteIO_GetError(void)
{
    MockDriver mock = {0};
    mock.error = IOERROR_BUFFER_OVERRUN_READ;
    ByteIO io = MakeStream(&mock);

    TEST_ASSERT_EQUAL_INT(IOERROR_BUFFER_OVERRUN_READ, ByteIO_GetError(&io));

    mock.error = IOERROR_NONE;
    TEST_ASSERT_EQUAL_INT(IOERROR_NONE, ByteIO_GetError(&io));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_ByteIO_Write_Valid);
    RUN_TEST(Test_ByteIO_Write_Partial);
    RUN_TEST(Test_ByteIO_Write_Empty);
    RUN_TEST(Test_ByteIO_Read_Valid);
    RUN_TEST(Test_ByteIO_Read_Eof);
    RUN_TEST(Test_ByteIO_IsBusy_True);
    RUN_TEST(Test_ByteIO_IsBusy_False);
    RUN_TEST(Test_ByteIO_GetError);
    return UNITY_END();
}
