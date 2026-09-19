// ────────────────────────────────────────────────────────────────────────────
// CLI — line-oriented command-line interface built on TextIO.
// ────────────────────────────────────────────────────────────────────────────
//
// Description:
//
// A small command dispatcher: a `CLI` owns a name, a sentinel-terminated list
// of `CLIParser`s (the last entry has `name == NULL`) and a silent flag.
// `CLI_HandleCommand` splits a line into whitespace-separated arguments and
// calls the matching parser; unmatched lines go to the currently active parser
// (the last one that matched). `CLI_Task` reads one line and dispatches it.
//
// The built-in commands `exit` (reset the active parser), `silent on|off`
// and `echo <text>` are handled before the active-parser fallback.
//
// Usage:
//
//   static Bool Print(TextIO io[const static 1], u8 number_of_args,
//                     String args[number_of_args], Any context)
//   {
//       UNUSED(context);
//       for(u8 i = 0; i < number_of_args; i++) TextIO_Write(io, args[i]);
//       return true;
//   }
//
//   static const CLIParser parsers[] = {
//       {"print", NULL, Print, NULL},
//       {0},
//   };
//
//   CLI cli = NEW_CLI("my-cli", parsers, false);
//   CLI_SayHello(&cli, &console);
//   while(CLI_IDLE != CLI_Task(&cli, &console)) { }
// ────────────────────────────────────────────────────────────────────────────

#ifndef KOLEKTO_CLI
#define KOLEKTO_CLI

#include "Datatypes.h"
#include "TextIO.h"

// ────────────────────────────────────────────────────────────────────────────
// Data Types
// ────────────────────────────────────────────────────────────────────────────

/** @brief Maximum number of whitespace-separated arguments on one line. */
#define CLI_MAX_ARGUMENTS 16

/** @brief Result of handling a command line. */
typedef enum
{
    CLI_IDLE,
    CLI_COMMAND_HANDLED,
    CLI_UNKNOWN_COMMAND,
} CLIStatus;

/**
 * @brief A named command.
 *
 * `help` and `parse` follow the function conventions. `parse` receives the
 * number of arguments and the argument vector after the command name, plus the
 * parser's own `context`.
 */
typedef struct
{
    String name;
    void (*help)(TextIO io[const static 1]);
    Bool (*parse)(TextIO io[const static 1], u8 number_of_args, String args[number_of_args], Any context);
    Any context;
} CLIParser;

/** @brief Line-oriented command-line interface. */
typedef struct
{
    String name;
    CLIParser const *parser_list;
    CLIParser const *last_parser;
    Bool silent;
} CLI;

/**
 * @brief Statically initialize a `CLI`.
 * @param name_value CLI name used by `CLI_SayHello`.
 * @param parser_list_value Sentinel-terminated parser list (`{0}` last).
 * @param silent_mode Suppress prompts and greetings when true.
 */
#define NEW_CLI(name_value, parser_list_value, silent_mode) \
    (CLI){.name = (name_value), \
          .parser_list = (parser_list_value), \
          .last_parser = NULL, \
          .silent = (silent_mode)}

/**
 * @brief Read one line from `io` and dispatch it.
 * @param cli CLI holding the parser list.
 * @param io Stream to read from.
 * @return `CLI_IDLE` on an empty/EOF line, otherwise the dispatch result.
 */
CLIStatus CLI_Task(CLI cli[const static 1], TextIO io[const static 1]);

/**
 * @brief Dispatch a command line against the CLI's parser list.
 * @param cli CLI holding the parser list.
 * @param io Stream passed to the parser.
 * @param command Null-terminated command line.
 * @return `CLI_COMMAND_HANDLED`, `CLI_UNKNOWN_COMMAND`, or `CLI_IDLE` when
 *         the line is empty.
 */
CLIStatus CLI_HandleCommand(CLI cli[const static 1], TextIO io[const static 1], String command);

/**
 * @brief Print the CLI name and a greeting, unless silent.
 * @param cli CLI to greet with.
 * @param io Stream to write to.
 */
void CLI_SayHello(CLI cli[const static 1], TextIO io[const static 1]);

/**
 * @brief Enable or disable silent mode.
 * @param cli CLI to update.
 * @param silent_mode_on New silent flag value.
 */
void CLI_SilentMode(CLI cli[const static 1], Bool silent_mode_on);

#endif /* KOLEKTO_CLI */
