#include <stdarg.h>		// for va_<xxx>
#include <stdio.h>		// for vsprintf_s()
#include <Windows.h>	// for OutputDebugStringA(). Uggh.. this pulls in a lot of Windows specific stuff

namespace Engine
{

void ConsolePrint( const char * i_fmt, ... ) 
{
	const unsigned int str_length = 128;

	char str[str_length];

	// define a variable argument list variable 
    va_list args;
	
	// initialize it. second parameter is variable immediately
	// preceeding variable arguments
    va_start( args, i_fmt );

	// print our formatted string to a char buffer (safely)
	vsprintf_s( str, str_length, i_fmt, args );

	va_end( args );

    OutputDebugStringA( str );
}

} // Namespace Engine