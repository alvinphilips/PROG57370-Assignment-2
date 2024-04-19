#include "EngineCore.h"
#include "RenderSystem.h"
#include "IRenderable.h"
#include "Scene.h"

void RenderSystem::Initialize(const bool create_renderer)
{
	//Pulls the window information from the RenderSettings file located in Assets
	std::ifstream inputStream("../Assets/RenderSettings.json");
	std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	json::JSON document = json::JSON::Load(str);

	//Checks for name in RenderSettings
	if (document.hasKey("name"))
	{
		title = document["name"].ToString();
	}
	else
	{
		std::cout << "Name wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for width in RenderSettings
	if (document.hasKey("width"))
	{
		width = document["width"].ToInt();
	}
	else
	{
		std::cout << "Width wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for height in RenderSettings
	if (document.hasKey("height"))
	{
		height = document["height"].ToInt();
	}
	else
	{
		std::cout << "Height wasn't found in RenderSettings. Going with the Default instead.";
	}

	//Checks for fullscreen in RenderSettings
	if (document.hasKey("fullscreen"))
	{
		is_fullscreen = document["fullScreen"].ToBool();
	}
	else
	{
		std::cout << "Fullscreen wasn't found in RenderSettings. Going with the Default instead.";
	}

	if (create_renderer) {
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, is_fullscreen);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		debug_layer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);
		SDL_SetTextureBlendMode(debug_layer, SDL_BLENDMODE_BLEND);
	}
}

void RenderSystem::Destroy()
{
	if (debug_layer != nullptr)
	{
		SDL_DestroyTexture(debug_layer);
	}

	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
	}
}


void RenderSystem::Update()
{
	if (renderer == nullptr) return;

	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);

	for (auto renderable : renderables)
	{
		if (!renderable->GetComponent()->GetOwner()->GetParentScene()->IsEnabled())
		{
			continue;
		}
		renderable->Render();
	}

	// Draw any Debug information
	SDL_RenderCopy(renderer, debug_layer, nullptr, nullptr);

	// Clear Debug information
	SDL_SetRenderTarget(renderer, debug_layer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, nullptr);

	SDL_RenderPresent(renderer);

#ifndef DEBUG_FPS_COUNTER
	static float accumulated_delta_time = 0;
	accumulated_delta_time += Time::Instance().DeltaTime();

	if (Time::Instance().FrameCount() % 1000 == 0)
	{
		SDL_SetWindowTitle(window, ("FPS: " + std::to_string(1000.0f / accumulated_delta_time)).c_str());
		accumulated_delta_time = 0;
	}
#endif
}

void RenderSystem::Load()
{

}

SDL_Window& RenderSystem::GetWindow()
{
	return *window;
}

SDL_Renderer& RenderSystem::GetRenderer()
{
	return *renderer;
}

SDL_Texture* RenderSystem::GetDebugLayer()
{
	return debug_layer;
}

void RenderSystem::AddRenderable(IRenderable* renderable)
{
	renderables.push_back(renderable);
}

void RenderSystem::RemoveRenderable(IRenderable* renderable)
{
	renderables.remove(renderable);
}

void RenderSystem::WindowBackgroundColor(int r, int g, int b, int a)
{
	background_color.r = r;
	background_color.g = g;
	background_color.b = b;
	background_color.a = a;
}

void RenderSystem::WindowSize(const int new_width, const int new_height)
{
	if (!is_fullscreen)
	{
		width = new_width;
		height = new_height;

		if (window != nullptr)
		{
			SDL_SetWindowSize(window, width, height);
		}
	}
	else
	{
		std::cout << "Can't resize a fullscreen window. Change the RenderSettings if you want to use this method." << std::endl;
	}
}

IVec2 RenderSystem::GetWindowSize() const
{
	return { static_cast<int>(width), static_cast<int>(height) };
}
