#include "../../stdafx.h"
#include "../../utility/utility.h"
#include "../../utility/files.h"

#include "duk_extensions.h"

#include <string>
#include <stdio.h>

#include <duktape.h>
#include <easylogging++.h>
#include <duk_module_node.h>

#ifdef __WINDOWS__
	#include <windows.h>
	#include <stdlib.h>
	#include <shlwapi.h>
	#include <atlstr.h>

	#define FILE_SEP "\\"
#endif

// This function will have to be modified for xbox consoles
#ifdef __WINDOWS__
// TODO: Create logic for the node_moduls directory
std::string ResolveNodeJSModuleFilePath(const char* module) {
	LOG(FATAL) << "ResolveNodeJSModuleFilePath not implemented";

	return "";
}

std::string GetScriptDirRoot() {
	return Files::GetActiveExecutablePath();
}
#endif

bool ResolveJavascriptPath(duk_context* ctx, std::string& path, const char* parent_id, const char* module_id) {
	// Local file, Relative path
	if(strncmp(module_id, "./", 2) == 0 || strncmp(module_id, ".\\", 2) == 0) 
	{
		if (strlen(module_id) < 3) {
			LOG(FATAL) << "Cannot resolve module: " << module_id;
			(void) duk_type_error(ctx, "cannot load module none specified: %s", module_id);
		} 

		char szFolderPath[MAX_PATH];
		std::string strFilePath;

		// We are using the root directory
		if (strlen(parent_id) == 0) {
			strcpy_s(szFolderPath, sizeof(szFolderPath), GetScriptDirRoot().c_str());
			Files::GetBasePath(szFolderPath);

			// Build the file path
			strFilePath.append(szFolderPath);
			strFilePath.append(FILE_SEP);
			strFilePath.append(module_id + 2);
			strFilePath.append(".js");
			strFilePath = Files::ResolvePath(strFilePath.c_str());
		} 
		
		// We are resolving a subfolder
		else {
			LOG(WARNING) << __FILE__ << ":" << __LINE__ << " parent_id != EMPTY, " << parent_id;

			strcpy_s(szFolderPath, sizeof(szFolderPath), parent_id);
			Files::GetBasePath(szFolderPath);

			// Build the file path
			strFilePath.append(szFolderPath);
			strFilePath.append(FILE_SEP);
			strFilePath.append(module_id + 2);
			strFilePath.append(".js");
			strFilePath = Files::ResolvePath(strFilePath.c_str());
		}

		const char* szGeneratedFilePath = strFilePath.c_str();
		path = strFilePath;

		return true;
	} else {
		// Node directory
		path = ResolveNodeJSModuleFilePath(module_id);
		return true;
	}

	return false;
}

/**
 * Resolves module id
 */
static duk_ret_t cb_resolve_module(duk_context *ctx) {
	const char *module_id;
	const char *parent_id;
	module_id = duk_require_string(ctx, 0);
	parent_id = duk_require_string(ctx, 1);

	std::string FilePath;
	if (ResolveJavascriptPath(ctx, FilePath, parent_id, module_id)) {
		duk_push_string(ctx, FilePath.c_str());

		// printf("resolve_cb, found: id:'%s', parent-id:'%s', resolve-to:'%s'\n",
		// 	module_id, parent_id, duk_get_string(ctx, -1));
		return 1;
	}

	LOG(FATAL) << "Cannot resolve module: " << module_id;
	(void) duk_type_error(ctx, "cannot find module: %s", module_id);

	return 0;
}

static duk_ret_t cb_load_module(duk_context *ctx) {
	const char *filename;
	const char *module_id;

	module_id = duk_require_string(ctx, 0);
	duk_get_prop_string(ctx, 2, "filename");
	filename = duk_require_string(ctx, -1);

	LOG(TRACE) << "load_cb: Loading javascript file id: " << module_id;
	if (Files::Exists(filename)) {
		std::string source = Files::Read(filename);
		duk_push_lstring(ctx, source.c_str(), source.length());
		return 1;
	} 
		
	LOG(ERROR) << "File does not exists: " << filename;
	(void) duk_type_error(ctx, "cannot find module: %s", module_id);
	return 1;
}

/**
 * Must be called only ONCE!
 */
void duk_ext_module_loader(duk_context *ctx) {
    /* After initializing the Duktape heap or when creating a new
    * thread with a new global environment:
    */
    duk_push_object(ctx);
    duk_push_c_function(ctx, cb_resolve_module, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "resolve");
    duk_push_c_function(ctx, cb_load_module, DUK_VARARGS);
    duk_put_prop_string(ctx, -2, "load");
    duk_module_node_init(ctx);
}