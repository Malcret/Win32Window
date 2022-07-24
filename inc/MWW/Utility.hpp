#pragma once

#include <iostream>
#include <Windows.h>

#define MWW_BIT(x) (1 << (x))

#ifndef NDEBUG
#define MWW_ASSERT(x, msg) if (!x) { std::cerr << "Assertion Failed: \"" << __FILE__ << ":" << __LINE__ << "\": " << msg; __debugbreak(); }
#define MWW_HANDLE_HRESULT(x, msg) if (FAILED(x)) { std::cerr << "HRESULT Failed: \"" << __FILE__ << ":" << __LINE__ << "\": " << msg; __debugbreak(); }
#else
#define MWW_ASSERT(x, msg) x;
#define MWW_HANDLE_HRESULT(x, msg) x;
#endif // NDEBUG

namespace MWW {
	double getTime();
	double getDeltaTime();
	unsigned int getFPS(double deltaTime);
}