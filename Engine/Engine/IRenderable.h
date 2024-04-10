#pragma once

#ifndef _I_RENDERABLE_H_
#define _I_RENDERABLE_H_

class IRenderable
{
protected:
	IRenderable();
	virtual ~IRenderable();

	virtual void Render() = 0;
	virtual Component* GetComponent() = 0;

	friend class RenderSystem;
};

#endif
