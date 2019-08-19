#include "../stdafx.h"

#include <iostream>
#include <vector>
#include <string>

#include <duktape.h>
#include <duk_console.h>
#include <duk_print_alert.h>
#include <dukglue/dukglue.h>
#include <easylogging++.h>

#include "../utility/utility.h"
#include "../utility/files.h"
#include "../application/application.h"
#include "../scripting/binding/classes.h"
#include "../scripting/extensions/duk_extensions.h"

void dk_tests(duk_context* ctx);

int application(int argc, char *argv[]) {
    Application app;

    main_argc = argc;
    main_argv = argv;
    
    return 0;
}

Application::Application() {
    std::cout << "Initializing ctx" << std::endl;
    ctx = nullptr;

    initialize();
    binding();
    run();
}

Application::~Application() {
    std::cout << "Cleaning ctx" << std::endl;
    cleanup();
}

bool Application::initialize() {
    ctx = duk_create_heap_default();
    if (!ctx) { exit(1); }

    // Initialize optional features
    duk_console_init(ctx, DUK_CONSOLE_FLUSH);
    duk_ext_module_loader(ctx);

    return true;
}

bool Application::binding() {
    BindingClasses::CreateBindingsForClasses(ctx);
    return true;
}

bool Application::cleanup() {
	duk_destroy_heap(ctx);

    return true;
}

void Application::run() {
    // Create the global variable (instance)
    dh_create_global_variable(ctx);
    dk_tests(ctx);
   

    // Load the file and execute it.
    std::string fileSrc = Files::Read("scripts/application.js");
    if (duk_peval_lstring(ctx, fileSrc.c_str(), fileSrc.length())) {
        // if an error occured while executing, print the stack trace
        duk_get_prop_string(ctx, -1, "stack");
        std::cout << duk_safe_to_string(ctx, -1) << std::endl;
        duk_pop(ctx);

        printf("There was an error running the script!\n");
    } else {
        // Pop eval result
        duk_pop(ctx);

        // Call the function PluginLoop(), No args. Expected result = BOOL / NUMBER
        if (duk_get_global_string(ctx, "PluginLoop")) {
            duk_call(ctx, 0);
            duk_int_t retType = duk_get_type(ctx, -1);
            if (retType != DUK_TYPE_UNDEFINED && (retType == DUK_TYPE_BOOLEAN || retType == DUK_TYPE_NUMBER)) {
                duk_int32_t result;

                if (retType == DUK_TYPE_BOOLEAN) 
                    result = duk_to_boolean(ctx, -1) ? 1 : 0;
                else result = duk_to_int32(ctx, -1);
                
                printf("Result from PluginLoop = %d\n", result);
            } else {
                printf("Unknown or no result returned from function. (Result Type = %d)\n", retType);
            }


            printf("Script finished successfully!\n");
        } else {
            printf("Script finished without finding PluginLoop function!\n");
        }

        // Pop function result (PluginLoop)
        duk_pop(ctx);
    }
}


// static duk_ret_t testFunction(duk_context *ctx) {
//     printf("testFunction\n");
//     duk_push_number(ctx, 55);
//     return 1;
// }

// int testFunctionNoDuk() {
//     printf("testFunctionNoDuk\n");

//     return 56;
// }

void dk_tests(duk_context* ctx) {
    // {
    //     // Attempt to create a new object
    //     //duk_idx_t renderer_idx;
    //     BindingClasses::Rendering rend;
    //     dukglue_push(ctx, &rend);
    //     duk_put_global_string(ctx, "Render");

    //     // dh_print_stack_to_stdout(ctx);
    // }
    // {
    //     printf("Before creating object\n       "); dh_print_stack_to_stdout(ctx);


    //     // Attempt to create a global object and attach a function
    //     // duk_get_global_string(ctx, "Game");


    //     // duk_idx_t idx = duk_push_object(ctx);
    //     // printf("Object index = %d\n", idx);

    //     // printf("Before creating function (testFunction)\n       "); dh_print_stack_to_stdout(ctx);
    //     // duk_push_c_function(ctx, testFunction, 0);
	//     // duk_put_prop_string(ctx, -2, "TestFunction");
    //     // printf("After creating function (testFunction)\n       "); dh_print_stack_to_stdout(ctx);

    //     duk_get_global_string(ctx, "Game");
    //     dukglue_register_function_property(ctx, &testFunctionNoDuk, "TestFunctionNoDuk");
    //     duk_pop(ctx);

    //     printf("After registering func property \n       "); dh_print_stack_to_stdout(ctx);



    //     // duk_put_global_string(ctx, "TestObject");
    //     printf("After creating object\n       "); dh_print_stack_to_stdout(ctx);
    // }
}
