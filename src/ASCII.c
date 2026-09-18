#include "ASCII.h"


#define	_U	01
#define	_L	02
#define	_N	04
#define	_S	010
#define _P	020
#define _C	040
#define _X	0100
#define	_B	0200

u8 ascii_table[128] = {
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, 
	_C,	_C|_S, _C|_S, _C|_S,	_C|_S,	_C|_S,	_C,	_C, 
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, 
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C, 
	_S|_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P, 
	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P, 
	_N,	_N,	_N,	_N,	_N,	_N,	_N,	_N, 
	_N,	_N,	_P,	_P,	_P,	_P,	_P,	_P, 
	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U, 
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U, 
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U, 
	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P, 
	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L, 
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L, 
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L, 
	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C
};

Bool ASCII_IsValid(ASCII character)
{
    return character > 0;
}

Bool ASCII_IsAlphaNumeric(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_U|_L|_N);
}

Bool ASCII_IsLetter(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_U|_L);
}

Bool ASCII_IsLowerCase(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_L);
}

Bool ASCII_IsUpperCase(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_U);
}

Bool ASCII_IsDigit(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_N);
}

Bool ASCII_IsHexDigit(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_N|_X);
}

Bool ASCII_IsPrintable(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_P|_U|_L|_N|_B);
}

Bool ASCII_IsSpace(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_S);
}

Bool ASCII_IsPunctuation(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_P);
}

Bool ASCII_IsControlCode(ASCII character)
{
    return ascii_table[(unsigned char)character] & (_C);
}

ASCII ASCII_ToLower(ASCII character)
{
    if(ASCII_IsUpperCase(character))
    {
        return character - 'A' + 'a';
    }
    return character;
}

ASCII ASCII_ToUpper(ASCII character)
{
    if(ASCII_IsLowerCase(character))
    {
        return character - 'a' + 'A';
    }
    return character;
}
