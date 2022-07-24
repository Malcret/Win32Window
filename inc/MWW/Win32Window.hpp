#pragma once

#include <string>
#include <Windows.h>

#include "Event/KeyboardEvent.hpp"
#include "Event/MouseEvent.hpp"
#include "Event/WindowEvent.hpp"

namespace MWW {
	class Win32Window {
	public:
		Win32Window();
		Win32Window(const std::string &title, int width, int height);
		Win32Window(const Win32Window &) = delete;
		~Win32Window();

		Win32Window &operator=(const Win32Window &) = delete;

		void close(int exitCode = 0);

		void pollEvents();
		void waitEvents();
		void swapBuffers();

		void setTitle(const std::string &title);
		void setIcon(const std::string &icon);
		void setSize(int width, int height);
		void setPosition(int x, int y);
		void setResizable(bool resizable = true);
		void setFullscreen(bool fullscreen = true);
		void setMaximized(bool maximized = true);
		void setMinimized(bool minimized = true);
		void setVisible(bool visible = true);
		void setFocused(bool focused = true);

		bool shouldClose() const;
		HINSTANCE getHInstance() const;
		HWND getHWND() const;
		HDC getHDC() const;
		std::string getTitle() const;
		int getWidth() const;
		int getHeight() const;
		float getAspectRatio() const;
		int getXPos() const;
		int getYPos() const;
		bool isResizable() const;
		bool isFullscreen() const;
		bool isMaximized() const;
		bool isMinimized() const;
		bool isVisible() const;
		bool isFocused() const;
		int getExitCode() const;

		bool keyPressed(KEY key) const;
		bool buttonPressed(BUTTON button) const;

		inline void setEventCallback(EventCallbackFunc callback) { m_eventCallback = callback; }

	private:
		typedef struct {
			DWORD style;
			int width;
			int height;
			int xPos;
			int yPos;
			bool resizable;
			bool maximized;
		} SavedProps;

		const wchar_t *m_CLASS_NAME;
		HINSTANCE m_hInstance;
		HWND m_hWnd;
		HDC m_hDC;
		DWORD m_style;
		std::string m_title;
		int m_width, m_height, m_xPos, m_yPos;
		float m_aspectRatio;
		bool m_resizable, m_fullscreen, m_maximized, m_minimized, m_visible, m_focused;
		bool m_shouldClose;
		int m_exitCode;

		bool m_keyPressed[255];
		bool m_buttonPressed[6];

		EventCallbackFunc m_eventCallback;

		void handleEvents(MSG &msg);

		LRESULT msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};
}