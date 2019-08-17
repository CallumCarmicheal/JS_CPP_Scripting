#pragma once

#include <duk_config.h>

class Application {
public:
    Application();
    ~Application();

private:
    duk_context* ctx;

private:
    bool binding();
    bool initialize();
    bool cleanup();

    void run();
};

int application(int argc, char *argv[]);