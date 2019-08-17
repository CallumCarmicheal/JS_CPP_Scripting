#pragma once

#include <string>
#include <duk_config.h>

namespace BindingClasses {

class Colour {
public:
    int a,r,g,b;

    int getA() { return a; }
    void setA(int v) { a=v; }

    int getR() { return r; }
    void setR(int v) { r=v; }
    
    int getG() { return g; }
    void setG(int v) { g=v; }
    
    int getB() { return b; }
    void setB(int v) { b=v; }

public:
    Colour() : a(255), r(255), g(255), b(255) {}
    Colour(int a, int r, int g, int b) : a(a), r(r), g(g), b(b) {}

    unsigned int getHexColour();
    unsigned int getHexColourAlpha();

    std::string toString();
};


class Rendering {
public:
    void DrawLineARGB(int x, int y, int r, int g, int b, int a);
    void DrawLineC(int x, int y, Colour* colour);
};

bool CreateBindingsForClasses(duk_context* ctx);

}