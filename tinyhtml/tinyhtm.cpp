#include "stdafx.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "tinyhtm.h"

const char *TIHTMVERSION = "intyhtml-0.1";

const char *TiHtmVersion()
{
	return TIHTMVERSION;
}
static FILE *TiHtmOpen(const char *filename, const char *mode)
{
	FILE* fp = NULL;
	fopen_s(&fp, filename, mode);
	return fp;
}

// the scope of class TiHtmBase

void TiHtmBase::encodeString(const std::string& str, std::string* outString)
{
	int i = 0, len = str.length();
	
	while (i < len)
	{
		unsigned char c = (unsigned char)str[i];
		
		if (c == '&' && (i < len - 2) && (str[i+1] == '#') && (str[i+2] == 'x'))
		{
			// Hexadecimal character reference.
			// Pass through unchanged.
			// &#xA9;	-- copyright symbol, for example.
			//
			// The -1 is a bug fix from Rob Laveaux. It keeps
			// an overflow from happening if there is no ';'.
			// There are actually 2 ways to exit this loop -
			// while fails (error case) and break (semicolon found).
			// However, there is no mechanism (currently) for
			// this function to return an error.
			while (i < len - 1)
			{
				outString->append(str.c_str() + i, 1);
				++i;
				if ( str[i] == ';' )
					break;
			}
		}
		else if (c == '&')
		{
			outString->append(entity[0].str, entity[0].strLength);
			++i;
		}
		else if ( c == '<' )
		{
			outString->append(entity[1].str, entity[1].strLength);
			++i;
		}
		else if ( c == '>' )
		{
			outString->append(entity[2].str, entity[2].strLength);
			++i;
		}
		else if ( c == '\"' )
		{
			outString->append(entity[3].str, entity[3].strLength);
			++i;
		}
		else if ( c == '\'' )
		{
			outString->append(entity[4].str, entity[4].strLength);
			++i;
		}
		// 特殊不显示的字符
		else if (c < 32)
		{
			// Easy pass at non-alpha/numeric/symbol
			// Below 32 is symbolic.
			char buf[ 32 ];
			
			snprintf(buf, sizeof(buf), "&#x%02X;", (unsigned)( c & 0xff ));

			//*ME:	warning C4267: convert 'size_t' to 'int'
			//*ME:	Int-Cast to make compiler happy ...
			outString->append( buf, (int)strlen( buf ) );
			++i;
		}
		else
		{
			*outString += (char)c;
			++i;
		}
	}
}

