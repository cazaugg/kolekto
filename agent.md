# Kolekto — Naming Conventions

This document describes the naming conventions used in the Kolekto C23 library.

Principles

- Use correct english spelling
- Make names short and clear
- Avoid abbreviations where possible and where not generally known
  - write string instead of str
  - Write copy instead ok cpy
  - Well known abbreviations (outside of programming) are acceptable (eg Systems like GPS or Units like ms, kg, mm)
  - Abbreviations are always all cap
- Be consistent
- Well justified exceptions are acceptable

## Files and Modules

- Each module gets a name, it written in pascal case.
- File names equal to the module name unless this results in a conflict with the C standard library.

## Types

- Except few basic types in `Datatypes.h` types are named pascal case without the _t prefix
- For void pointers use the `Any` type.
- Object like structures provide a `NEW_OBJECT_NAME` macro for static initialization

> _t prefix is reserved by the C standard

## Functions

- All public functions use the module name in pascal case as a prefix followed by an underscore.
- All public functions have a documentation comment in the header
- All internal functions are marked static and do not use the module prefix.
- All internal functions get a declaration and documentation before the implementation sections
- Array parameters carry their length in a preceding parameter and use the VLA-style `const` qualifier: `String list[nof_joins]`

## Macros and Constants

- Enums and macros use all cap spelling
- Function like macros for generic type handling may use function naming conventions

## Variables, Parameters, Struct Members

- Snake case

## Tests (Unity)

- Test functions names: `Test_<Unit>_<Case>` in `PascalCase`,
  declared `static void`, registered in `main` via `RUN_TEST`.
  `Test_ASCII_IsValid_True` (`test/test_ascii.c:12`),
  `Test_ParseNumber_U8_Boundaries` (`test/test_parse_numbers.c:22`).
- `setUp` / `tearDown` must keep their exact lowercase names (Unity
  requirement) and are defined in every test file even when empty.
- Test names describe the case, not the assertion: suffix with `_True`,
  `_False`, `_Valid`, `_Invalid`, `_Empty`, `_Boundaries`, `_ExactCapacity`,
  `_InsufficientCapacity`.
- Assertions use Unity's typed macros: `TEST_ASSERT_EQUAL_UINT8`,
  `TEST_ASSERT_EQUAL_STRING`, `TEST_ASSERT_TRUE`/`_FALSE`.
