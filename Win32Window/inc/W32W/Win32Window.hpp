#pragma once

#include <string>
#include <Windows.h>

#include "Event/KeyboardEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"

namespace W32W {
	class Win32Window {
	public:
		Win32Window();
		Win32Window(const std::string &title, int width, int height);
		Win32Window(const Win32Window &) = delete;
		~Win32Window();

		Win32Window &operator=(const Win32Window &) = delete;

		void close(int exitCode = 0);

		void show();
		void pollEvents();
		void waitEvents();
		void swapBuffers();

		void setTitle(const std::string &title);

		bool shouldClose() const;
		HINSTANCE getHInstance() const;
		HWND getHWND() const;
		HDC getHDC() const;
		std::string getTitle() const;
		int getWidth() const;
		int getHeight() const;
		float getAspectRatio() const;
		int getExitCode() const;

		inline void setEventCallback(EventCallbackFunc callback) { m_eventCallback = callback; }

	private:
		const wchar_t *m_CLASS_NAME;
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		HDC m_hDC;
		std::string m_title;
		int m_width, m_height;
		float m_aspectRatio;
		bool m_shouldClose;
		int m_exitCode;

		EventCallbackFunc m_eventCallback;

		void handleEvents(MSG &msg);

		LRESULT msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}