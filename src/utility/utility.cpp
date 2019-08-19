#include <duktape.h>
#include <iostream>
#include <vector>
#include <string>
#include <easylogging++.h>

#include "utility.h"

void dh_print_stack_to_stdout(duk_context *ctx, const char* prefix) {
    duk_push_context_dump(ctx);
    LOG(DEBUG) << prefix << duk_safe_to_string(ctx, -1);

	//fprintf(stdout, "%s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);
}

void dh_print_stack_to_stdout(duk_context *ctx) {
    duk_push_context_dump(ctx);
    LOG(DEBUG) << duk_safe_to_string(ctx, -1);

	//fprintf(stdout, "%s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);
}

void dh_create_global_variable(duk_context *ctx) {
    if (duk_peval_string(ctx, 
        "if (typeof global === 'undefined') {"
        "    (function () {"
        "        var global = new Function('return this;')();"
        "        Object.defineProperty(global, 'global', {"
        "            value: global,"
        "            writable: true,"
        "            enumerable: false,"
        "            configurable: true"
        "        });"
        "    })();"
        "}"
    )) {
        // if an error occured while executing, print the stack trace
        duk_get_prop_string(ctx, -1, "stack");
        std::cout << duk_safe_to_string(ctx, -1) << std::endl;

        printf("There was an error creating a global object instance!\n");
    }

    duk_pop(ctx);
}

/**
 * Call javascript function without parameters and return the result type
 */
duk_int_t dh_call_p0(duk_context* ctx, const char* globalName) {
    duk_get_global_string(ctx, globalName);
    duk_call(ctx, 0);
    return duk_get_type(ctx, -1);
}

/**
 * Get the response type from function call DUK_TYPE
 */
duk_int_t dh_get_call_response_type(duk_context* ctx) {
    return duk_get_type(ctx, -1);
}

#ifdef __WINDOWS__
#include <Windows.h>
void __SetStdConsoleTextAttributes(int k) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
#endif