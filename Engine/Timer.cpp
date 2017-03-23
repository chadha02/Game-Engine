#include "Timer.h"
#include "../GLib/_Console/ConsolePrint.h"

namespace Engine
{
	namespace Time
	{

		bool Timer::Init()
		{
			LARGE_INTEGER i;
			if (!QueryPerformanceFrequency(&i))
				return false;

			frequencySec = (float)(i.QuadPart);
			QueryPerformanceCounter(&i);
			start = i.QuadPart; // get the ticks
			totalTime = 0;
			deltaTime = 0;
			return true;
		}

		void Timer::Update()
		{
			LARGE_INTEGER i;
			QueryPerformanceCounter(&i);
			deltaTime = (float)(i.QuadPart - start) / frequencySec;
			start = i.QuadPart;
			totalTime += deltaTime;
			
			//return deltaTime;
		}

		
	}
}