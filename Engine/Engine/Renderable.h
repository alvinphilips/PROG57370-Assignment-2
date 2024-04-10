#pragma once

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Component.h"
#include "SDL.h"

class Renderable : public Component
{
protected:
	Renderable();
	virtual ~Renderable();

private:
	virtual void Render() = 0;

	friend class RenderSystem;
};

#endif
