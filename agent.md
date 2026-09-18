# Kolekto — Naming Conventions

This document describes the naming conventions used in the Kolekto C23 library.

## Implementation

- Never use dynamic memory allocation
- Object-like structures provide a `NEW_OBJECT_NAME` macro for static initialization
- Use VLA notation for arrays as function arguments
- Use VLA notation for pointer as function arguments (`[const static 1]`)

## Style

Principles

- Use correct English spelling
- Make names short and clear
- Avoid abbreviations where possible and where not generally known
  - write string instead of str
  - Write copy instead of cpy
  - Well known abbreviations (outside of programming) are acceptable (eg Systems like GPS or Units like ms, kg, mm)
  - Abbreviations are always all cap, except in snake case situations (variables)
- Be consistent
- Well justified exceptions are acceptable

### Files and Modules

- Each module gets a name, it is written in pascal case.
- File names equal to the module name unless this results in a conflict with the C standard library. (Eg `SafeStrings.h` is the file name, but the module is named/prefixed `String`)
  - If there are generic functions used, the prefix is extended with type `ModuleType_` for the implementation. The macro uses the same name with the module prefix.
- Header guards are all cap and start with `KOLEKTO_`

### Types

- Except a few basic types in `Datatypes.h`, types are named pascal case without the `_t` suffix. The exception is the lowercase scalar aliases `u8`…`u64`, `i8`…`i64`, `f32`, `f64`; everything else is pascal case.
- For void pointers use the `Any` type.

> _t suffix is reserved by the C standard

### Functions

- All public functions use the module name in pascal case as a prefix followed by an underscore.
- All public functions have a documentation comment in the header
- All internal functions are marked static and do not use the module prefix.
- All internal functions get a declaration and documentation before the implementation sections
- Array parameters carry their length in a preceding parameter and use the VLA-style `const` qualifier: `String list[nof_joins]`
- Function pointer are written according to these function conventions.

### Macros Enums and Constants

- Enums and macros use all cap spelling
- Enums are prefixed with their enum or type name
- Function like macros for generic type handling may use function naming conventions

### Variables, Parameters, Struct Members

- Snake case

### Tests (Unity)

- Test file names are `Test_<Unit>`
- Test functions names: `Test_<Unit>_<Case>` in `PascalCase`,
  declared `static void`, registered in `main` via `RUN_TEST`.
  `Test_ASCII_IsValid_True` (`test/TestASCII.c:12`),
  `Test_ParseNumber_U8_Boundaries` (`test/TestParseNumbers.c:22`).
- `setUp` / `tearDown` must keep their exact lowercase names (Unity
  requirement) and are defined in every test file even when empty.
- Test names describe the case, not the assertion: suffix with `_True`,
  `_False`, `_Valid`, `_Invalid`, `_Empty`, `_Boundaries`, `_ExactCapacity`,
  `_InsufficientCapacity`.
- Assertions use Unity's typed macros: `TEST_ASSERT_EQUAL_UINT8`,
  `TEST_ASSERT_EQUAL_STRING`, `TEST_ASSERT_TRUE`/`_FALSE`.
