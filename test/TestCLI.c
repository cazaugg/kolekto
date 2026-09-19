// ────────────────────────────────────────────────────────────────────────────
// TestCLI — Unity tests for the CLI module.
// ────────────────────────────────────────────────────────────────────────────

#include "unity.h"
#include "../src/CLI.h"

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

typedef struct
{
    u8 calls;
    u8 last_argc;
    String last_arg0;
} ParserContext;

static ParserContext context;

static Bool ParsePrint(TextIO io[const static 1], u8 number_of_args, String args[number_of_args], Any context_data)
{
    ParserContext *state = (ParserContext*) context_data;
    state->calls++;
    state->last_argc = number_of_args;
    state->last_arg0 = (number_of_args > 0) ? args[0] : "";

    for(u8 i = 0; i < number_of_args; i++)
    {
        TextIO_Write(io, args[i]);
        if(i + 1 < number_of_args) TextIO_Write(io, " ");
    }
    TextIO_LineBreak(io);
    return true;
}

static Bool ParseReject(TextIO io[const static 1], u8 number_of_args, String args[number_of_args], Any context_data)
{
    UNUSED(io);
    UNUSED(number_of_args);
    UNUSED(args);
    UNUSED(context_data);
    return false;
}

static void HelpPrint(TextIO io[const static 1])
{
    TextIO_WriteLine(io, "print <text>");
}

static const CLIParser parsers[] = {
    {"reject", NULL, ParseReject, &context},
    {"print", HelpPrint, ParsePrint, &context},
    {0},
};

static void SetInput(MockDriver *mock, String text)
{
    Size length = String_Length(text);
    for(Size i = 0; i < length; i++)
    {
        mock->input[i] = (u8) text[i];
    }
    mock->input_count = length;
}

void setUp(void)
{
    context = (ParserContext){0};
}

void tearDown(void)
{
}

static void Test_CLI_SayHello_NotSilent(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, false);

    CLI_SayHello(&cli, &io);
    TEST_ASSERT_EQUAL_STRING("This is the my-cli CLI\nmy-cli>\n", (char*) mock.output);
}

static void Test_CLI_SayHello_Silent(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    CLI_SayHello(&cli, &io);
    TEST_ASSERT_EQUAL_size_t(0, mock.output_count);
}

static void Test_CLI_SilentMode(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    CLI_SayHello(&cli, &io);
    TEST_ASSERT_EQUAL_size_t(0, mock.output_count);

    CLI_SilentMode(&cli, false);
    CLI_SayHello(&cli, &io);
    TEST_ASSERT_TRUE(mock.output_count > 0);
}

static void Test_CLI_HandleCommand_Known(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "print hello world"));
    TEST_ASSERT_EQUAL_UINT8(1, context.calls);
    TEST_ASSERT_EQUAL_UINT8(2, context.last_argc);
    TEST_ASSERT_EQUAL_STRING("hello", context.last_arg0);
}

static void Test_CLI_HandleCommand_NoArgs(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "print"));
    TEST_ASSERT_EQUAL_UINT8(0, context.last_argc);
    TEST_ASSERT_EQUAL_STRING("", context.last_arg0);
}

static void Test_CLI_HandleCommand_Unknown(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_UNKNOWN_COMMAND, CLI_HandleCommand(&cli, &io, "bogus arg"));
    TEST_ASSERT_EQUAL_size_t(0, mock.output_count);
    TEST_ASSERT_EQUAL_UINT8(0, context.calls);
}

static void Test_CLI_HandleCommand_UnknownNotSilent(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, false);

    TEST_ASSERT_EQUAL_INT(CLI_UNKNOWN_COMMAND, CLI_HandleCommand(&cli, &io, "bogus arg"));
    TEST_ASSERT_EQUAL_STRING("Unknown command: bogus arg\n", (char*) mock.output);
}

static void Test_CLI_HandleCommand_Empty(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_IDLE, CLI_HandleCommand(&cli, &io, "   "));
}

static void Test_CLI_HandleCommand_Rejected(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_UNKNOWN_COMMAND, CLI_HandleCommand(&cli, &io, "reject"));
}

static void Test_CLI_HandleCommand_BuiltinSilent(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, false);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "silent on"));
    TEST_ASSERT_TRUE(cli.silent);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "silent off"));
    TEST_ASSERT_FALSE(cli.silent);
}

static void Test_CLI_HandleCommand_BuiltinEcho(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, false);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "echo a b"));
    TEST_ASSERT_EQUAL_STRING("a b\n", (char*) mock.output);
}

static void Test_CLI_HandleCommand_ExitResetsActiveParser(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "print x"));
    TEST_ASSERT_EQUAL_UINT8(1, context.calls);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "garbage"));
    TEST_ASSERT_EQUAL_UINT8(2, context.calls);

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_HandleCommand(&cli, &io, "exit"));
    TEST_ASSERT_EQUAL_INT(CLI_UNKNOWN_COMMAND, CLI_HandleCommand(&cli, &io, "garbage"));
    TEST_ASSERT_EQUAL_UINT8(2, context.calls);
}

static void Test_CLI_Task_Dispatch(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);
    SetInput(&mock, "print hi\n");

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_Task(&cli, &io));
    TEST_ASSERT_EQUAL_UINT8(1, context.calls);
    TEST_ASSERT_EQUAL_UINT8(1, context.last_argc);
    TEST_ASSERT_EQUAL_STRING("hi", context.last_arg0);
}

static void Test_CLI_Task_Prompt(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, false);
    SetInput(&mock, "print hi\n");

    TEST_ASSERT_EQUAL_INT(CLI_COMMAND_HANDLED, CLI_Task(&cli, &io));
    TEST_ASSERT_EQUAL_STRING("hi\nmy-cli> \n", (char*) mock.output);
}

static void Test_CLI_Task_Eof(void)
{
    MockDriver mock = {0};
    mock.output_limit = 64;
    ByteIO stream = NEW_BYTE_IO(&mock, MockWriteByte, MockReadByte, MockGetStatus);
    TextIO io = NEW_TEXT_IO(stream, 64, "\n", ECHO_OFF);
    CLI cli = NEW_CLI("my-cli", parsers, true);

    TEST_ASSERT_EQUAL_INT(CLI_IDLE, CLI_Task(&cli, &io));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(Test_CLI_SayHello_NotSilent);
    RUN_TEST(Test_CLI_SayHello_Silent);
    RUN_TEST(Test_CLI_SilentMode);
    RUN_TEST(Test_CLI_HandleCommand_Known);
    RUN_TEST(Test_CLI_HandleCommand_NoArgs);
    RUN_TEST(Test_CLI_HandleCommand_Unknown);
    RUN_TEST(Test_CLI_HandleCommand_UnknownNotSilent);
    RUN_TEST(Test_CLI_HandleCommand_Empty);
    RUN_TEST(Test_CLI_HandleCommand_Rejected);
    RUN_TEST(Test_CLI_HandleCommand_BuiltinSilent);
    RUN_TEST(Test_CLI_HandleCommand_BuiltinEcho);
    RUN_TEST(Test_CLI_HandleCommand_ExitResetsActiveParser);
    RUN_TEST(Test_CLI_Task_Dispatch);
    RUN_TEST(Test_CLI_Task_Prompt);
    RUN_TEST(Test_CLI_Task_Eof);
    return UNITY_END();
}
