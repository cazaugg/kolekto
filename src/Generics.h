#ifndef KOLEKTO_GENERICS
#define KOLEKTO_GENERICS

#include "Datatypes.h"

/** @brief Tag describing which union member of `Generic` is active. */
typedef enum : u8
{
    GENERIC_NULL = 0,
    GENERIC_U32,
    GENERIC_I32,
    GENERIC_BOOL,
    GENERIC_FLOAT,
    GENERIC_STRING,
} GenericType;

/** @brief Dynamically typed value. */
typedef struct
{
    GenericType type;
    Size count;
    Any range;
    union
    {
        u32 u32_value;
        i32 i32_value;
        Bool boolean;
        float f32_value;
        String string;
    } value;
} Generic;

/**
 * @brief Check whether a value can be interpreted as `target_type`.
 * @return true when the generic already holds the requested type.
 */
static inline Bool Generic_CanCast(Generic generic, GenericType target_type)
{
    return generic.type == target_type;
}

/** @brief Store an unsigned 8-bit value; returns true on success. */
static inline Bool Generic_SetU8(Generic generic[const static 1], u8 value)
{
    ASSERT_OR(generic) return false;
    generic[0].type = GENERIC_U32;
    generic[0].value.u32_value = value;
    return true;
}

/** @brief Store a boolean value; returns true on success. */
static inline Bool Generic_SetBool(Generic generic[const static 1], Bool value)
{
    ASSERT_OR(generic) return false;
    generic[0].type = GENERIC_BOOL;
    generic[0].value.boolean = value;
    return true;
}

/** @brief Return the `GenericType` that corresponds to `u32`. */
static inline GenericType Generic_GetTypeU32(u32 value)
{
    UNUSED(value);
    return GENERIC_U32;
}

/**
 * @brief Check whether a `GENERIC_U32` holds a value within `[min, max]`.
 * @return true when the stored value is in range.
 */
static inline Bool Generic_CheckRangeU32(Generic generic[const static 1], u32 min, u32 max)
{
    ASSERT_OR(generic && generic[0].type == GENERIC_U32) return false;
    return generic[0].value.u32_value >= min && generic[0].value.u32_value <= max;
}

#endif /* KOLEKTO_GENERICS */
