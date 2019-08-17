#pragma once

// ============ CONSTANTS ============
//#define JS_ENGINE_PRINT_CONSTANT_STREAM(str) str					// Uncomment to just print the js text without JS ( )
#define JS_ENGINE_PRINT_CONSTANT_STREAM(str) "JS " << str			// Surround JS with ( ), This will appear in the output! Used for debugging

#define CONFIG_LOG_FILE_ENABLED		"true"							// Needs to be a string, Defines logging for NON DEBUG. 
																	// If set to false logging will still be enabled for debug
#define CONFIG_LOG_FILE_NAME		"application.log"				// The file, can be full path to a location

#ifdef __DEBUG__
	#define CONFIG_LOG_FORMAT		"%datetime{%Y-%M-%d %H:%m:%s,%g} %level %msg"	// Defines the default format for the application when debugging
#else
	#define CONFIG_LOG_FORMAT		"datetime{%Y-%M-%d %H:%m:%s,%g} %level %msg"	// Defines the default format for the application
#endif

#define CONFIG_LOG_PRINT_TO_STDOUT	"true"											// States if the application prints to STDOUT by default

// ============ FUNCTIONS ============

void CONFIGURATION_LIBRARIES();
void CONFIGURATION_LIBRARIES(int argc, char *argv[]);

void CONFIGURATION_CLEANUP();