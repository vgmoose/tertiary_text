#include <pebble.h>

// A callback which is called when a user submits data
//
// Params:
// result - The data the user typed. Memory is shared across calls to tertiary_text_prompt, so copy it before calling again
// result_lengh - Number of characters in result
// extra - The extra data passed to tertiary_text_prompt
//
typedef void(* TertiaryTextCallback)( const char* result, size_t result_length, void* extra );

// Opens a tertiary text window and gets user input
//
// Params:
// title - Some text to display on the top of the input screen
// callback - The function to call when input is complete
// extra - Some extra data to pass to the callback
//
void tertiary_text_prompt( const char* title, TertiaryTextCallback callback, void* extra );
