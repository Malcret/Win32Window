#pragma once

#include <iostream>
#include <Windows.h>

#define W32W_BIT(x) (1 << (x))

#ifndef NDEBUG
#define W32W_ASSERT(x, msg) if (!x) { std::cerr << "Assertion Failed: \"" << __FILE__ << ":" << __LINE__ << "\": " << msg; __debugbreak(); }
#define W32W_HANDLE_HRESULT(x, msg) if (FAILED(x)) { std::cerr << "HRESULT Failed: \"" << __FILE__ << ":" << __LINE__ << "\": " << msg; __debugbreak(); }
#else
#define W32W_ASSERT(x, msg) x;
#define W32W_HANDLE_HRESULT(x, msg) x;
#endif // NDEBUG

namespace W32W {
	double getTime();
	double getDeltaTime();
	unsigned int getFPS(double deltaTime);
}