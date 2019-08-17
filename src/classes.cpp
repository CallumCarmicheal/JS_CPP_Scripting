#include <sstream>

#include <easylogging++.h>
#include <duktape.h>
#include <duk_console.h>
#include <dukglue/dukglue.h>

#include "headers/utility.h"
#include "headers/classes.h"
#include "headers/configuration.h"

using namespace BindingClasses;

unsigned int Colour::getHexColour()      { return (r<<16) + (g<<8) + b; }
unsigned int Colour::getHexColourAlpha() { return (a<<24) + (r<<16) + (g<<8) + b; }

std::string Colour::toString() {
    std::ostringstream s;
    s << ("Colour(a: ") << a << ", r: " << r << ", g: " << g << ", b: " << b << ")";

    return s.str();
}

void Rendering::DrawLineARGB(int x, int y, int r, int g, int b, int a) {
    printf("Rendering::DrawLine(%d, %d, %d, %d, %d, %d)\n", x, y, r, g, b, a);
}

void Rendering::DrawLineC(int x, int y, Colour* colour) {
    printf("Rendering::DrawLine(%d, %d, %s)\n", x, y, colour->toString().c_str());
}

static std::string internal__javascript_logger(duk_context *ctx, const char *error_name) {
	duk_idx_t n = duk_get_top(ctx);
	duk_idx_t i;

	duk_get_global_string(ctx, "console");
	duk_get_prop_string(ctx, -1, "format");

	for (i = 0; i < n; i++) {
		if (duk_check_type_mask(ctx, i, DUK_TYPE_MASK_OBJECT)) {
			/* Slow path formatting. */
			duk_dup(ctx, -1);  /* console.format */
			duk_dup(ctx, i);
			duk_call(ctx, 1);
			duk_replace(ctx, i);  /* arg[i] = console.format(arg[i]); */
		}
	}

	duk_pop_2(ctx);

	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, n);

	if (error_name) {
		duk_push_error_object(ctx, DUK_ERR_ERROR, "%s", duk_require_string(ctx, -1));
		duk_push_string(ctx, "name");
		duk_push_string(ctx, error_name);
		duk_def_prop(ctx, -3, DUK_DEFPROP_FORCE | DUK_DEFPROP_HAVE_VALUE);  /* to get e.g. 'Trace: 1 2 3' */
		duk_get_prop_string(ctx, -1, "stack");
	}

	return std::string(duk_to_string(ctx, -1));
}

static duk_ret_t native_logger(duk_context *ctx) {
	duk_idx_t n = duk_get_top(ctx);

    // If we dont have any args
    if (n == 0) { return 0; }
    if (n == 1) { // If we have an arg thats not a number then we can print it
        if (duk_check_type(ctx, 0, DUK_TYPE_NUMBER)) return 0;
        LOG(INFO) << internal__javascript_logger(ctx, NULL); 
        return 0;
    }

    // check if the type is a int
    el::Level level = el::Level::Global;

    if (duk_check_type(ctx, 0, DUK_TYPE_NUMBER)) {
        int ilvl = duk_get_int(ctx, 0);
        duk_remove(ctx, 0);
        std::string output = internal__javascript_logger(ctx, NULL);

        switch (ilvl) {
        // DEF = INFO
        // 0   = INFO
        // 1   = TRACE
        // 2   = DEBUG
        // 3   = WARNING
        // 4   = ERROR
        default:
        case 0:  LOG(INFO) << JS_ENGINE_PRINT_CONSTANT_STREAM(output); break;
        case 1:  LOG(TRACE) << JS_ENGINE_PRINT_CONSTANT_STREAM(output); break;
        case 2:  LOG(DEBUG) << JS_ENGINE_PRINT_CONSTANT_STREAM(output); break;
        case 3:  LOG(WARNING) << JS_ENGINE_PRINT_CONSTANT_STREAM(output); break;
        case 4:  LOG(ERROR) << JS_ENGINE_PRINT_CONSTANT_STREAM(output); break;
        }
    } else {
        std::string output = internal__javascript_logger(ctx, NULL);
        LOG(INFO) << JS_ENGINE_PRINT_CONSTANT_STREAM(output);
    }

	return 0;
}

bool BindingClasses::CreateBindingsForClasses(duk_context* ctx) {
    // Printer
    duk_push_c_function(ctx, native_logger, DUK_VARARGS);
	duk_put_global_string(ctx, "log");

    // Colour
    dukglue_register_constructor<Colour>(ctx, "Colour");
    dukglue_register_constructor<Colour, int, int, int, int>(ctx, "Colour");

    dukglue_register_method(ctx, &Colour::getHexColour, "getHexColour");
    dukglue_register_method(ctx, &Colour::getHexColourAlpha, "getHexColourAlpha");
    dukglue_register_method(ctx, &Colour::toString, "toString");

    dukglue_register_property(ctx, &Colour::getA, &Colour::setA, "a");
    dukglue_register_property(ctx, &Colour::getR, &Colour::setR, "r");
    dukglue_register_property(ctx, &Colour::getG, &Colour::setG, "g");
    dukglue_register_property(ctx, &Colour::getB, &Colour::setB, "b");

    // Renderer
    dukglue_register_constructor<Rendering>(ctx, "Rendering");
    dukglue_register_method(ctx, &Rendering::DrawLineARGB, "DrawLineARGB");
    dukglue_register_method(ctx, &Rendering::DrawLineC, "DrawLineC");

    return true;
}