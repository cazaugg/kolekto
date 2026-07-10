#include "ascii.h"


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

bool ASCII_IsValid(ascii character)
{
    return character < 128;
}

bool ASCII_IsAlphaNumeric(ascii character)
{
    return ascii_table[character] & (_U|_L|_N);
}

bool ASCII_IsLetter(ascii character)
{
    return ascii_table[character] & (_U|_L);
}

bool ASCII_IsLowerCase(ascii character)
{
    return ascii_table[character] & (_L);
}

bool ASCII_IsUpperCase(ascii character)
{
    return ascii_table[character] & (_U);
}

bool ASCII_IsDigit(ascii character)
{
    return ascii_table[character] & (_N);
}

bool ASCII_IsHexDigit(ascii character)
{
    return ascii_table[character] & (_N|_X);
}

bool ASCII_IsPrintable(ascii character)
{
    return ascii_table[character] & (_P|_U|_L|_N|_B);
}

bool ASCII_IsSpace(ascii character)
{
    return ascii_table[character] & (_S);
}

bool ASCII_IsPunctuation(ascii character)
{
    return ascii_table[character] & (_P);
}

bool ASCII_IsControlCode(ascii character)
{
    return ascii_table[character] & (_C);
}

ascii ASCII_ToLower(ascii character)
{
    if(ASCII_IsUpperCase(character))
    {
        return character - 'A' + 'a';
    }
    return character;
}

ascii ASCII_ToUpper(ascii character)
{
    if(ASCII_IsLowerCase(character))
    {
        return character - 'a' + 'A';
    }
    return character;
}
