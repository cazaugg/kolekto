// ────────────────────────────────────────────────────────────────────────────
// CLI — line-oriented command-line interface built on TextIO.
// ────────────────────────────────────────────────────────────────────────────

#include "CLI.h"

// ── Internal functions ───────────────────────────────────────────────────────

/**
 * @brief Find the first parser whose name matches.
 * @param name Command name to look up.
 * @param parser_list Sentinel-terminated parser list.
 * @return The matching parser, or NULL when none matches.
 */
static CLIParser const *FindParser(String name, CLIParser const *parser_list);

/**
 * @brief Split a mutable line into whitespace-separated arguments.
 * @param command Line to split (modified in place).
 * @param max_args Capacity of `args`.
 * @param args Receives the argument pointers.
 * @return The number of arguments produced.
 */
static u8 SplitArgs(MutableString *command, u8 max_args, String args[max_args]);

/**
 * @brief Write `number_of_args` arguments separated by spaces, then a break.
 * @param io Stream to write to.
 * @param number_of_args Number of arguments to print.
 * @param args Arguments to print.
 */
static void PrintArgs(TextIO io[const static 1], u8 number_of_args, String args[number_of_args]);

// ── Public functions ─────────────────────────────────────────────────────────

CLIStatus CLI_Task(CLI cli[const static 1], TextIO io[const static 1])
{
    if(0 == TextIO_ReadLine(io)) return CLI_IDLE;

    CLIStatus state = CLI_HandleCommand(cli, io, io->line_buffer.data);
    if(!cli->silent) TextIO_PrintLine(io, "%s> ", cli->name);
    return state;
}

CLIStatus CLI_HandleCommand(CLI cli[const static 1], TextIO io[const static 1], String command)
{
    if(command != io->line_buffer.data)
    {
        if(0 == String_Set(&io->line_buffer, command)) return CLI_IDLE;
    }

    String_Trim(&io->line_buffer);
    if(String_IsEmpty(io->line_buffer)) return CLI_IDLE;

    String args[CLI_MAX_ARGUMENTS];
    u8 nof_args = SplitArgs(&io->line_buffer, CLI_MAX_ARGUMENTS, args);
    u8 number_of_args = (nof_args > 0) ? (u8)(nof_args - 1) : 0;

    if(cli->last_parser == NULL) cli->last_parser = cli->parser_list;

    CLIParser const *parser = FindParser(args[0], cli->parser_list);
    Bool handled = false;

    if(parser != NULL)
    {
        cli->last_parser = parser;
        handled = parser->parse(io, number_of_args, args + 1, parser->context);
    }
    else if(String_Equal(args[0], "exit"))
    {
        cli->last_parser = cli->parser_list;
        handled = true;
    }
    else if(String_Equal(args[0], "silent"))
    {
        String mode = (number_of_args > 0) ? args[1] : "";
        if(String_Equal(mode, "on")) cli->silent = true;
        else if(String_Equal(mode, "off")) cli->silent = false;
        handled = true;
    }
    else if(String_Equal(args[0], "echo"))
    {
        PrintArgs(io, number_of_args, args + 1);
        handled = true;
    }
    else if(cli->last_parser != NULL && cli->last_parser->parse != NULL)
    {
        handled = cli->last_parser->parse(io, number_of_args, args + 1, cli->last_parser->context);
    }

    if(handled) return CLI_COMMAND_HANDLED;

    if(!cli->silent)
    {
        TextIO_Write(io, "Unknown command: ");
        PrintArgs(io, nof_args, args);
    }
    return CLI_UNKNOWN_COMMAND;
}

void CLI_SayHello(CLI cli[const static 1], TextIO io[const static 1])
{
    if(cli->silent) return;

    TextIO_PrintLine(io, "This is the %s CLI", cli->name);
    TextIO_PrintLine(io, "%s>", cli->name);
}

void CLI_SilentMode(CLI cli[const static 1], Bool silent_mode_on)
{
    cli->silent = silent_mode_on;
}

// ── Internal implementation ──────────────────────────────────────────────────

static CLIParser const *FindParser(String name, CLIParser const *parser_list)
{
    for(CLIParser const *parser = parser_list; parser->name != NULL; parser++)
    {
        if(String_Equal(parser->name, name)) return parser;
    }
    return NULL;
}

static u8 SplitArgs(MutableString *command, u8 max_args, String args[max_args])
{
    return String_Split(command, " \t", max_args, args);
}

static void PrintArgs(TextIO io[const static 1], u8 number_of_args, String args[number_of_args])
{
    for(u8 i = 0; i < number_of_args; i++)
    {
        TextIO_Write(io, args[i]);
        if(i + 1 < number_of_args) TextIO_Write(io, " ");
    }
    TextIO_LineBreak(io);
}
