#include "EngineCore.h"
#include "Renderable.h"
#include "RenderSystem.h"

Renderable::Renderable()
{
	RenderSystem::Instance().AddRenderable(this);
}

Renderable::~Renderable()
{
	RenderSystem::Instance().RemoveRenderable(this);
}
