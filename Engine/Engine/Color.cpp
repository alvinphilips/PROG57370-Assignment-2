#include "EngineCore.h"
#include "Color.h"

#include <SDL_render.h>

#define BYTE_TO_FLOAT(byte) float(byte) / 255.0f

namespace gfx
{
    Color::Color()
    {
        *this = WHITE;
    }

    Color::Color(const uint32_t rgb, const uint8_t a)
    {
        // rgb field layout
        // | 00 - 07 | 08 - 15 | 16 - 23 | 24 - 31 | bits
        // |  blue   |  green  |   red   | unused  | component
        this->r = rgb >> 16 & 0xff;
        this->g = rgb >> 8 & 0xff;
        this->b = rgb & 0xff;
        this->a = a;
    }

    Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    Color::Color(SDL_Renderer* renderer)
    {
        SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    }

    const Color Color::WHITE = Color(255, 255, 255);
    const Color Color::BLACK = Color(0, 0, 0);
    const Color Color::RED = Color(255, 0, 0);
    const Color Color::GREEN = Color(0, 255, 0);
    const Color Color::BLUE = Color(0, 0, 255);
    const Color Color::MAGENTA = Color(255, 0, 255);
}