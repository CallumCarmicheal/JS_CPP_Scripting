
#include <duktape.h>
#include <duk_console.h>
#include <duk_print_alert.h>
#include <dukglue/dukglue.h>

#include "../headers/globals.h"
#include "duk_extensions.h"
#include "duk_trans_socket.h"

static int debugger_reattach = 1;

static duk_idx_t debugger_request(duk_context *ctx, void *udata, duk_idx_t nvalues) {
	const char *cmd;
	int i;

	(void) udata;

	if (nvalues < 1) {
		duk_push_string(ctx, "missing AppRequest argument(s)");
		return -1;
	}

	cmd = duk_get_string(ctx, -nvalues + 0);

	if (cmd && strcmp(cmd, "CommandLine") == 0) {
		if (!duk_check_stack(ctx, main_argc)) {
			/* Callback should avoid errors for now, so use
			 * duk_check_stack() rather than duk_require_stack().
			 */
			duk_push_string(ctx, "failed to extend stack");
			return -1;
		}
		for (i = 0; i < main_argc; i++) {
			duk_push_string(ctx, main_argv[i]);
		}
		return main_argc;
	}
	duk_push_sprintf(ctx, "command not supported");
	return -1;
}

static void debugger_detached(duk_context *ctx, void *udata) {
	fprintf(stderr, "Debugger detached, udata: %p\n", (void *) udata);
	fflush(stderr);

	/* Ensure socket is closed even when detach is initiated by Duktape
	 * rather than debug client.
	 */
	duk_trans_socket_finish();

	if (debugger_reattach) {
		/* For automatic reattach testing. */
		duk_trans_socket_init();
		duk_trans_socket_waitconn();
		fprintf(stderr, "Debugger reconnected, call duk_debugger_attach()\n");
		fflush(stderr);

    #if 0
		/* This is not necessary but should be harmless. */
		duk_debugger_detach(ctx);
    #endif

		duk_debugger_attach(ctx,
            duk_trans_socket_read_cb,
            duk_trans_socket_write_cb,
            duk_trans_socket_peek_cb,
            duk_trans_socket_read_flush_cb,
            duk_trans_socket_write_flush_cb,
            debugger_request,
            debugger_detached,
            NULL);
	}
}


void duk_ext_debugger_start(duk_context* ctx) {
    // duk_console_init(ctx, DUK_CONSOLE_FLUSH);
    // fprintf(stderr, "Debugger enabled, create socket and wait for connection\n");
    // fflush(stderr);
    // duk_trans_socket_init();
    // duk_trans_socket_waitconn();
    // fprintf(stderr, "Debugger connected, call duk_debugger_attach() and then execute requested file(s)/eval\n");
    // fflush(stderr);
    // duk_debugger_attach(ctx,
    //     duk_trans_socket_read_cb,
    //     duk_trans_socket_write_cb,
    //     duk_trans_socket_peek_cb,
    //     duk_trans_socket_read_flush_cb,
    //     duk_trans_socket_write_flush_cb,
    //     debugger_request,
    //     debugger_detached,
    //     NULL);
}