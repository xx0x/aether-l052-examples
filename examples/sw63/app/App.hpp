#pragma once

#include "app/Display.hpp"

class App
{
public:
    void Init();
    void Loop();

private:
    Display display_;
};
