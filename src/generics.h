

#include "datatypes.h"

typedef enum : u8
{
    GENERIC_NULL = 0,
    GENERIC_U32,
    GENERIC_I32,
    GENERIC_Bool,
    GENERIC_FLOAT,
    GENERIC_STRING,
} GenericType;

Bool Generic_CanCast(Generic target, GenericType value)
{
    
}

typedef struct
{
    GenericType type;
    Size count;
    Any range;
    union
    {
        u32 u32;
        i32 i32;
        Bool Boolean;
        float f32;
        String str;
    } value;
} Generic;

Bool Generic_SetU8(Generic generic[const static 1], u8 value)
{
    if(Range_Fits())
}

Bool Generic_SetBool()

#define Generic_Get(generic)    generic->type == GENERIC_U32 ? generic->value.u32;
GenericType Generic_GetTypeU32(u32 val)
{
    UNUSED(val);
    return GENERIC_U32;
}

Bool Generic_Checkrange_u32(Generic generic[const static 1], u32 value)
{
    ASSERT(generic->type == value);
    if(generic->type != value) return false;


}
