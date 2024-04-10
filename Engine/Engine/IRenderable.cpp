#include "EngineCore.h"
#include "IRenderable.h"
#include "RenderSystem.h"

IRenderable::IRenderable()
{
	RenderSystem::Instance().AddRenderable(this);
}

IRenderable::~IRenderable()
{
	RenderSystem::Instance().RemoveRenderable(this);
}
