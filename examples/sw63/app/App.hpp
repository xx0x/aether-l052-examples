#pragma once

#include "app/Display.hpp"

class App
{
public:
    static void Init();
    static void Loop();

    inline static Display display;
};
