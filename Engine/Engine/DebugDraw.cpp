#include "EngineCore.h"
#include "DebugDraw.h"

#include "RenderSystem.h"

namespace gfx {
    void Debug::DrawRect(const Vec2& top_left, const Vec2& dimensions, const Color& color) {

        const Vec2 points[5] = {
        	top_left,
        	top_left + Vec2(dimensions.x, 0),
			top_left + dimensions,
            top_left + Vec2(0, dimensions.y),
        	top_left,
        };

        const auto renderer = &RenderSystem::Instance().GetRenderer();
        const auto debug_layer = RenderSystem::Instance().GetDebugLayer();

        const Color prev_color(renderer);

        SDL_SetRenderTarget(renderer, debug_layer);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLinesF(renderer, (const SDL_FPoint*) points, 5);
        SDL_SetRenderDrawColor(renderer, prev_color.r, prev_color.g, prev_color.b, prev_color.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void Debug::DrawPaddedRect(const Vec2& top_left, const Vec2& dimensions, const Vec2& pad)
    {
        DrawPaddedRect(top_left, dimensions, draw_color, pad);
    }

    void Debug::DrawPaddedRect(const Vec2& top_left, const Vec2& dimensions, const Color& color, const Vec2& pad)
    {
        const auto padded_top_left = top_left - pad;
        const auto padded_dimensions = dimensions + pad * 2;

        DrawRect(padded_top_left, padded_dimensions, color);
    }

    void Debug::DrawCircle(const Vec2& origin, const float radius, const Color& color, const unsigned int steps)
    {
        if (steps == 0)
        {
	        return;
        }
        const float circle_step = (MATH_PI * 2) / (float) steps;
        const auto renderer = &RenderSystem::Instance().GetRenderer();
        const auto debug_layer = RenderSystem::Instance().GetDebugLayer();

        const Color prev_color(renderer);

        SDL_SetRenderTarget(renderer, debug_layer);
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        for (float i = 0; i <= MATH_PI * 2; i += circle_step)
        {
	        const auto start_pos = Vec2(sinf(i), cosf(i)) * radius + Vec2(origin);
	        const auto end_pos = Vec2(sinf(i + circle_step), cosf(i + circle_step)) * radius + Vec2(origin);
            SDL_RenderDrawLineF(renderer, start_pos.x, start_pos.y, end_pos.x, end_pos.y);
        }
        SDL_SetRenderDrawColor(renderer, prev_color.r, prev_color.g, prev_color.b, prev_color.a);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    Color Debug::draw_color = Color(255, 255, 255);
    Vec2 Debug::padding = Vec2(5);
    unsigned int Debug::circle_steps = 6;
}
