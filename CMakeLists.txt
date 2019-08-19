cmake_minimum_required(VERSION 3.0.0)
project(application VERSION 0.1.0)

include(CTest)
enable_testing()

# Output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


##### ========== PROJECT SETTINGS ==========
set(PROJECT_FILES
    # Source files
    src/application/application.h
    src/application/application.cpp
    src/application/renderer.h
    src/application/renderer.cpp

    src/scripting/binding/classes.h
    src/scripting/binding/classes.cpp

    src/scripting/extensions/duk_extensions.h
    src/scripting/extensions/duk_module_loader.cpp
    src/scripting/extensions/duk_debugger.cpp
    src/scripting/extensions/duk_trans_socket.h
    src/scripting/extensions/duk_trans_socket_windows.cpp

    src/utility/files.h
    src/utility/files.cpp
    src/utility/utility.h
    src/utility/utility.cpp
    
    src/configuration_def.h
    src/configuration.h
    src/configuration.cpp
    src/globals.h
    src/globals.cpp
    src/stdafx.h

    # Entry point
    src/main.cpp)

set (PROJECT_LIBS
    # Duktape
    libs/duktape/src/duk_config.h
    libs/duktape/src/duktape.h
    libs/duktape/src/duktape.c
    libs/duktape/extras/print-alert/duk_print_alert.h
    libs/duktape/extras/print-alert/duk_print_alert.c
    libs/duktape/extras/console/duk_console.h
    libs/duktape/extras/console/duk_console.c
    libs/duktape/extras/module-node/duk_module_node.h    
    libs/duktape/extras/module-node/duk_module_node.c

    
    # Dukglue
    libs/dukglue/include/dukglue/dukglue.h
    libs/dukglue/include/dukglue/detail_class_proto.h
    libs/dukglue/include/dukglue/detail_constructor.h
    libs/dukglue/include/dukglue/detail_function.h
    libs/dukglue/include/dukglue/detail_method.h
    libs/dukglue/include/dukglue/detail_primitive_types.h
    libs/dukglue/include/dukglue/detail_refs.h
    libs/dukglue/include/dukglue/detail_stack.h
    libs/dukglue/include/dukglue/detail_traits.h
    libs/dukglue/include/dukglue/detail_typeinfo.h
    libs/dukglue/include/dukglue/detail_types.h
    libs/dukglue/include/dukglue/dukvalue.h
    libs/dukglue/include/dukglue/dukexception.h
    libs/dukglue/include/dukglue/register_class.h
    libs/dukglue/include/dukglue/register_function.h
    libs/dukglue/include/dukglue/register_property.h
    libs/dukglue/include/dukglue/public_util.h
    
    # Easylogging ++
    libs/easylogging++/easylogging++.h
    libs/easylogging++/easylogging++.cc
    
    # Nlohmann Json
    libs/nlohmann_json/nlohmann/json.hpp)


add_executable(${PROJECT_NAME} ${PROJECT_LIBS} ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    libs/duktape/src
    libs/duktape/extras/console
    libs/duktape/extras/print-alert
    libs/duktape/extras/module-node
    
    libs/dukglue/include
    
    libs/easylogging++
    
    libs/nlohmann_json)

# PreProcessor Defs
target_compile_definitions(${PROJECT_NAME} PRIVATE
    __DEBUG__=1     # Debugging
    __WINDOWS__=1   # Windows def
    #__XBOX360__=1  # Xbox 360 def
    #__CYGWIN__=1   # Cygwin definition!
)



# CPack configuration
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

