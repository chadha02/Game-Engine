#pragma once

#include <Windows.h>
#define Desired_FPS  60.0f;

namespace Engine

{
	namespace Time
	{

		class Timer
		{
		public:
			float deltaTime;
			float totalTime;
			bool Init();
			void Update();
			

		private:
			LONGLONG start;
			float frequencySec;
		};
	}
}