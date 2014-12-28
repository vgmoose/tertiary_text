#include "tertiary_text.h"

void callback( const char* result, size_t result_length, void* extra )
{
	// Do something with result
	APP_LOG( APP_LOG_LEVEL_INFO, "User typed %s", result );
}

int main( void )
{
	tertiary_text_prompt( "What's your name?", callback, NULL );

	app_event_loop();
	
	return 0;
}
