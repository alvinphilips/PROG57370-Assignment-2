#include "EngineCore.h"
#include "EngineTime.h"

void Time::Initialize()
{
	begin_time = std::chrono::system_clock::now();
	end_time = std::chrono::system_clock::now();

	delta_time = std::chrono::duration<float>(0);
	total_time = std::chrono::duration<float>(0);
	frame_count = 0;
}

void Time::Destroy()
{
}

void Time::Update()
{
	end_time = std::chrono::system_clock::now();
	delta_time = end_time - begin_time;
	begin_time = end_time;
	total_time += delta_time;
	frame_count++;
}
