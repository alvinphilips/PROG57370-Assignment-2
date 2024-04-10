#pragma once

#ifndef _ENGINE_TIME_H_
#define _ENGINE_TIME_H_

#include <chrono>

class Time final
{
	DECLARE_SINGLETON(Time)

public:
	float DeltaTime() const { return delta_time.count(); }
	float TotalTime() const { return total_time.count(); }
	unsigned int FrameCount() const { return frame_count; }

private:
	void Initialize();
	void Update();
	void Destroy();

private:
	unsigned int frame_count = 0;
	std::chrono::duration<float> delta_time = std::chrono::duration<float>(0.0f);
	std::chrono::duration<float> total_time = std::chrono::duration<float>(0.0f);
	std::chrono::time_point<std::chrono::system_clock> begin_time;
	std::chrono::time_point<std::chrono::system_clock> end_time;

	friend class Engine;
};

#endif
