#pragma once

#ifndef ENGINE_COLOR_H_
#define ENGINE_COLOR_H_

#include <cstdint>

struct SDL_Renderer;

namespace gfx {
    struct Color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;

        explicit Color();
        explicit Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

        explicit Color(SDL_Renderer* renderer);

        // Use a 32-bit integer to assign the RGB value, with an optional 8-bit alpha value.
        explicit Color(uint32_t rgb, uint8_t a = 255);

        static const Color WHITE;
        static const Color BLACK;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color MAGENTA;
    };
}

#endif