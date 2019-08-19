#pragma once
#include <duk_config.h>

void dh_print_stack_to_stdout(duk_context *ctx, const char* prefix);
void dh_print_stack_to_stdout(duk_context *ctx);

/**
 * Call javascript function without parameters and return the result type
 */
duk_int_t dh_call_p0(duk_context* ctx, const char* globalName);

/**
 * Get the response type from function call DUK_TYPE
 */
duk_int_t dh_get_call_response_type(duk_context* ctx);

void dh_create_global_variable(duk_context* ctx);

// ========== WINDOWS SPECIFIC INFORMATION ================

#define FG_BLUE      0x0001 // text color contains blue.
#define FG_GREEN     0x0002 // text color contains green.
#define FG_RED       0x0004 // text color contains red.
#define FG_INTENSITY 0x0008 // text color is intensified.
#define BG_BLUE      0x0010 // background color contains blue.
#define BG_GREEN     0x0020 // background color contains green.
#define BG_RED       0x0040 // background color contains red.
#define BG_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.

#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.

#ifdef __WINDOWS__
    #define SetStdConsoleTextAttributes __SetStdConsoleTextAttributes
    void __SetStdConsoleTextAttributes(int k);
#else
    #define SetStdConsoleTextAttributes
#endif
