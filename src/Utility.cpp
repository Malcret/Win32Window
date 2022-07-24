#include "MWW/Utility.hpp"

namespace MWW {
	double getTime() {
		static uint64_t frequency = 0;
		static bool qpf = QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
		if (qpf) {
			static uint64_t currentTime;
			QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);
			return ((double)currentTime / (double)frequency);
		}
		else {
			return (double)GetTickCount64();
		}
	}

	double getDeltaTime() {
		static double lastTime = 0;
		static double currentTime = 0;
		currentTime = getTime();
		double deltaTime = (currentTime - lastTime);
		lastTime = currentTime;

		return deltaTime;
	}

	unsigned int getFPS(double deltaTime) {
		return (unsigned int)(1.0 / deltaTime);
	}
}