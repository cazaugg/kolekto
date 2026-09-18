// ────────────────────────────────────────────────────────────────────────────
// TestTextIO — Unity tests for the TextIO module.
// ────────────────────────────────────────────────────────────────────────────

#include "unity.h"
#include "../src/TextIO.h"

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

static void SetInput(MockDriver *mock, String text)
{
    Size length = (Size) strlen(text);
    memcpy(mock->input, text, length);
    mock->input_count = length;
}

void setUp(void)
{
}

void tearDown(void)
{
}

static void Test_TextIO_Write(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);

    TEST_ASSERT_EQUAL_size_t(3, TextIO_Write(&io, "abc"));
    TEST_ASSERT_EQUAL_size_t(3, mock.output_count);
    TEST_ASSERT_EQUAL_UINT8('a', mock.output[0]);
    TEST_ASSERT_EQUAL_UINT8('b', mock.output[1]);
    TEST_ASSERT_EQUAL_UINT8('c', mock.output[2]);
}

static void Test_TextIO_WriteLine(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);

    TEST_ASSERT_EQUAL_size_t(3, TextIO_WriteLine(&io, "ab"));
    TEST_ASSERT_EQUAL_size_t(3, mock.output_count);
    TEST_ASSERT_EQUAL_UINT8('a', mock.output[0]);
    TEST_ASSERT_EQUAL_UINT8('b', mock.output[1]);
    TEST_ASSERT_EQUAL_UINT8('\n', mock.output[2]);
}

static void Test_TextIO_ReadLine_Valid(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    SetInput(&mock, "hello\n");

    TEST_ASSERT_EQUAL_size_t(6, TextIO_ReadLine(&io));

    MutableString line = TextIO_GetLine(&io);
    TEST_ASSERT_EQUAL_STRING("hello\n", line.data);
}

static void Test_TextIO_ReadLine_Sequential(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    SetInput(&mock, "one\ntwo\n");

    TEST_ASSERT_EQUAL_size_t(4, TextIO_ReadLine(&io));
    TEST_ASSERT_EQUAL_STRING("one\n", TextIO_GetLine(&io).data);

    TEST_ASSERT_EQUAL_size_t(4, TextIO_ReadLine(&io));
    TEST_ASSERT_EQUAL_STRING("two\n", TextIO_GetLine(&io).data);
}

static void Test_TextIO_ReadLine_Eof(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);

    TEST_ASSERT_EQUAL_size_t(0, TextIO_ReadLine(&io));
    TEST_ASSERT_EQUAL_STRING("", TextIO_GetLine(&io).data);
}

static void Test_TextIO_ReadLine_Partial(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    SetInput(&mock, "abc");

    TEST_ASSERT_EQUAL_size_t(0, TextIO_ReadLine(&io));
    TEST_ASSERT_EQUAL_STRING("abc", TextIO_GetLine(&io).data);
}

static void Test_TextIO_ReadLine_Overlong(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 4, "\n", ECHO_OFF);
    SetInput(&mock, "abcdef\n");

    TEST_ASSERT_EQUAL_size_t(0, TextIO_ReadLine(&io));
    TEST_ASSERT_EQUAL_STRING("abcd", TextIO_GetLine(&io).data);
}

static void Test_TextIO_Read_Valid(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    ASCII data[3] = {0};

    memcpy(mock.input, "\x04\x05\x06", 3);
    mock.input_count = 3;

    TEST_ASSERT_EQUAL_size_t(3, TextIO_Read(&io, 3, data));
    TEST_ASSERT_EQUAL_UINT8(4, data[0]);
    TEST_ASSERT_EQUAL_UINT8(5, data[1]);
    TEST_ASSERT_EQUAL_UINT8(6, data[2]);
    TEST_ASSERT_EQUAL_size_t(0, mock.output_count);
    TEST_ASSERT_EQUAL_STRING("\x04\x05\x06", TextIO_GetLine(&io).data);
}

static void Test_TextIO_Read_Echo(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_ON);
    ASCII data[2] = {0};

    memcpy(mock.input, "\x01\x02", 2);
    mock.input_count = 2;

    TEST_ASSERT_EQUAL_size_t(2, TextIO_Read(&io, 2, data));
    TEST_ASSERT_EQUAL_size_t(2, mock.output_count);
    TEST_ASSERT_EQUAL_UINT8(1, mock.output[0]);
    TEST_ASSERT_EQUAL_UINT8(2, mock.output[1]);
}

static void Test_TextIO_Read_Eof(void)
{
    MockDriver mock = {0};
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    ASCII data[5] = {0};

    memcpy(mock.input, "\x01\x02", 2);
    mock.input_count = 2;

    TEST_ASSERT_EQUAL_size_t(2, TextIO_Read(&io, 5, data));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_TextIO_Write);
    RUN_TEST(Test_TextIO_WriteLine);
    RUN_TEST(Test_TextIO_ReadLine_Valid);
    RUN_TEST(Test_TextIO_ReadLine_Sequential);
    RUN_TEST(Test_TextIO_ReadLine_Eof);
    RUN_TEST(Test_TextIO_ReadLine_Partial);
    RUN_TEST(Test_TextIO_ReadLine_Overlong);
    RUN_TEST(Test_TextIO_Read_Valid);
    RUN_TEST(Test_TextIO_Read_Echo);
    RUN_TEST(Test_TextIO_Read_Eof);
    return UNITY_END();
}
