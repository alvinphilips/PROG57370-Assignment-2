#pragma once

#ifndef _ENGINE_TIME_H_
#define _ENGINE_TIME_H_

#include <chrono>

class Time
{
	DECLARE_SINGLETON(Time)

public:
	float DeltaTime() const { return deltaTime.count(); }
	float TotalTime() const { return totalTime.count(); }
	unsigned int FrameCount() const { return frameCount; }

private:
	void Initialize();
	void Update();
	void Destroy();

private:
	unsigned int frameCount = 0;
	std::chrono::duration<float> deltaTime = std::chrono::duration<float>(0.0f);
	std::chrono::duration<float> totalTime = std::chrono::duration<float>(0.0f);
	std::chrono::time_point<std::chrono::system_clock> beginTime;
	std::chrono::time_point<std::chrono::system_clock> endTime;

	friend class Engine;
};

#endif
