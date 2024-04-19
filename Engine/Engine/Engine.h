#pragma once

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <list>

class Engine
{
	DECLARE_SINGLETON(Engine)

public:
	void Initialize(int argc = 0, char* argv[] = nullptr);
	void Destroy();
	void GameLoop();

	bool isRunning = true;
};

#endif
