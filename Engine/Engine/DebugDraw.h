#pragma once
#include "Color.h"
#include "FontAsset.h"

namespace gfx
{
// A set of graphical debugging utilities
	struct Debug
	{
		// Default Color to draw with
		static Color draw_color;

		// Default padding amount
		static Vec2 padding;

		// Default Font to use
		static FontAsset font;

		// Default steps to use for drawing Circles
		static unsigned int circle_steps;

		static void DrawRect(const Vec2& top_left, const Vec2& dimensions, const Color& color = draw_color);
		static void DrawPaddedRect(const Vec2& top_left, const Vec2& dimensions, const Vec2& pad = padding);
		static void DrawPaddedRect(const Vec2& top_left, const Vec2& dimensions, const Color& color = draw_color, const Vec2& pad = padding);
		
		static void DrawCircle(const Vec2& origin, float radius, const Color& color = draw_color, unsigned int steps = circle_steps, float rotation = 0);
	};

}
