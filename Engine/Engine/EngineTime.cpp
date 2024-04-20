#include "EngineCore.h"
#include "EngineTime.h"

#include "NetworkEngine.h"
#include "RenderSystem.h"

void Time::Initialize()
{
	begin_time = std::chrono::system_clock::now();
	end_time = std::chrono::system_clock::now();
	delta_time = std::chrono::duration<float>(0);
	total_time = std::chrono::duration<float>(0);
	time_scale = 1;
	frame_count = 0;
}

void Time::Destroy()
{
}

void Time::Update()
{
	end_time = std::chrono::system_clock::now();
	delta_time = end_time - begin_time;
	if (delta_time.count() < 0.1666666f)
	{
		const int eepy_time = (int) (0.1666666f - delta_time.count()) * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(eepy_time));
		
		end_time = std::chrono::system_clock::now();
		delta_time = end_time - begin_time;
	}
	begin_time = end_time;
	total_time += delta_time;
	frame_count++;
}
