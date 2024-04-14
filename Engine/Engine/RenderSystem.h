#pragma once

#ifndef _RENDER_SYSTEM_H_
#define _RENDER_SYSTEM_H_

#include <list>
#include "SDL.h"

class IRenderable;

class RenderSystem final
{
	DECLARE_SINGLETON(RenderSystem)

public:
	SDL_Window& GetWindow();
	SDL_Renderer& GetRenderer();
	SDL_Texture* GetDebugLayer();

	void AddRenderable(IRenderable* renderable);
	void RemoveRenderable(IRenderable* renderable);
	void WindowBackgroundColor(int r, int g, int b, int a);
	void WindowSize(int width, int height);
	IVec2 GetWindowSize() const;

protected:
	void Load();
	void Initialize();
	void Update();
	void Destroy();

private:
	std::string title = "DEFAULT NAME";
	int width = 1280;
	int height = 720;
	bool is_fullscreen = false;
	SDL_Color background_color = { 0,0,0,255 };
	std::list<IRenderable*> renderables;
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* debug_layer = nullptr;

	friend class Engine;
};

#endif