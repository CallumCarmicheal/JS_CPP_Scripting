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
    src/headers/globals.h
    src/headers/configuration.h
    src/headers/configuration_def.h
    src/headers/utility.h
    src/headers/files.hpp
    src/headers/application.h
    src/headers/classes.h
    src/headers/renderer.h
    src/scripting/duk_trans_socket.h
    src/scripting/duk_extensions.h
    
    src/globals.cpp
    src/configuration.cpp
    src/utility.cpp
    src/application.cpp   
    src/classes.cpp
    src/renderer.cpp
    src/scripting/duk_trans_socket_windows.cpp
    src/scripting/duk_debugger.cpp
    src/scripting/duk_module_loader.cpp

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
    libs/easylogging++/easylogging++.cc)


add_executable(${PROJECT_NAME} ${PROJECT_LIBS} ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    libs/duktape/src
    libs/duktape/extras/console
    libs/duktape/extras/print-alert
    libs/duktape/extras/module-node
    
    libs/dukglue/include
    
    libs/easylogging++)

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

