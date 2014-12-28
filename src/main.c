#include "tertiary_text.h"

void callback( const char* result, size_t result_length, void* extra )
{
	// Do something with result
}

int main( void )
{
	tertiary_text_prompt( "What's your name?", callback, NULL );

	return 0;
}
