#include "MWW/Win32Window.hpp"
#include <windowsx.h>
#include "MWW/Utility.hpp"

#ifndef NDEBUG
#define IF_EVENT_CALLBACK(x) if (m_eventCallback) { x; } else { MWW_ASSERT(false, "Failed to call event callback") }
#elif
#define IF_EVENT_CALLBACK(x) if (m_eventCallback) { x; }
#endif // NDEBUG

namespace MWW {
	Win32Window::Win32Window()
		: m_CLASS_NAME(L""), m_hInstance(nullptr), m_hWnd(nullptr), m_hDC(nullptr), m_style(0), m_title(""), m_width(0),
		  m_height(0), m_xPos(0), m_yPos(0), m_aspectRatio(0), m_resizable(false), m_fullscreen(false), m_maximized(false),
		  m_minimized(false), m_visible(false), m_focused(false), m_shouldClose(0), m_exitCode(0), m_keyPressed(), m_buttonPressed() {}

	Win32Window::Win32Window(const std::string &title, int width, int height)
		: m_CLASS_NAME(L"Win32Window"), m_hInstance(GetModuleHandle(nullptr)), m_title(title), m_width(width),
		  m_height(height), m_aspectRatio((float)m_height / (float)m_width), m_xPos(0), m_yPos(0), m_resizable(true),
		  m_fullscreen(false), m_maximized(false), m_minimized(false), m_visible(false), m_focused(true), m_shouldClose(false),
		  m_exitCode(0), m_keyPressed(), m_buttonPressed() {
		WNDCLASSEX wndClass {};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.lpszClassName = m_CLASS_NAME;                           // Class name
		wndClass.lpszMenuName = nullptr;                                 // Menu
		wndClass.hInstance = m_hInstance;                                // Instance
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);                    // Icon
		wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);                  // Small icon
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                  // Cursor
		wndClass.lpfnWndProc = windowProc;                               // Callback
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // Background color
		wndClass.style = CS_OWNDC;                                       // Redraw on move, own DC for window
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		RegisterClassEx(&wndClass);

		m_style = WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		const std::wstring wTitle(m_title.begin(), m_title.end());

		RECT r{};
		r.bottom = m_height;
		r.right = m_width;
		AdjustWindowRect(&r, m_style, FALSE);

		// Create the window
		if (!(m_hWnd = CreateWindowEx(
			0,                // Additional styles
			m_CLASS_NAME,     // Class name
			wTitle.c_str(),   // Title
			m_style,          // Styles
			CW_USEDEFAULT,    // X position
			CW_USEDEFAULT,    // Y position
			r.right - r.left, // Width
			r.bottom - r.top, // Height
			NULL,             // Parent window
			NULL,             // Menu
			m_hInstance,      // Instance
			NULL              // Additional application data
			))) {
			MWW_ASSERT(false, "Failed to create window");
		}

		if (!(m_hDC = GetDC(m_hWnd))) {
			MWW_ASSERT(false, "Failed to get device context");
		}

		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	}

	Win32Window::~Win32Window() {
		ReleaseDC(m_hWnd, m_hDC);
		DestroyWindow(m_hWnd);
		UnregisterClass(m_CLASS_NAME, m_hInstance);
	}

	void Win32Window::close(int exitCode) {
		m_exitCode = exitCode;
		m_shouldClose = true;
		PostQuitMessage(exitCode);
	}

	void Win32Window::pollEvents() {
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			handleEvents(msg);
		}
	}

	void Win32Window::waitEvents() {
		MSG msg;
		GetMessage(&msg, NULL, 0, 0);
		handleEvents(msg);
	}

	void Win32Window::handleEvents(MSG &msg) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	void Win32Window::swapBuffers() {
		SwapBuffers(m_hDC);
	}

	bool Win32Window::shouldClose() const {
		return m_shouldClose;
	}

	void Win32Window::setTitle(const std::string &title) {
		m_title = title;
		std::wstring wTitle(m_title.begin(), m_title.end());
		SetWindowText(m_hWnd, wTitle.c_str());
	}

	void Win32Window::setIcon(const std::string &icon) {
		const std::wstring wIcon(icon.begin(), icon.end());
		HANDLE  hIcon = LoadImage(NULL, wIcon.c_str(), IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
		SendMessage(m_hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	}

	void Win32Window::setSize(int width, int height) {
		if (!m_fullscreen && !m_maximized) {
			RECT r{};
			r.right = width;
			r.bottom = height;
			AdjustWindowRect(&r, m_style, FALSE);
			SetWindowPos(m_hWnd, HWND_TOP, 0, 0, r.right - r.left, r.bottom - r.top, SWP_NOMOVE);
		}
	}

	void Win32Window::setPosition(int x, int y) {
		if (!m_fullscreen && !m_maximized) {
			RECT r{};
			r.top = y;
			r.left = x;
			AdjustWindowRect(&r, m_style, FALSE);
			SetWindowPos(m_hWnd, NULL, r.left, r.top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
		}
	}

	void Win32Window::setResizable(bool resizable) {
		if (!m_fullscreen && !m_maximized) {
			if (resizable && !m_resizable) {
				m_style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
				m_resizable = true;
			}
			else if (!resizable && m_resizable) {
				m_style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
				m_resizable = false;
			}
			SetWindowLong(m_hWnd, GWL_STYLE, m_style);
		}
	}

	void Win32Window::setFullscreen(bool fullscreen) {
		static SavedProps sp{};
		if (fullscreen && !m_fullscreen) {
			RECT rcWnd{};
			GetWindowRect(m_hWnd, &rcWnd);
			sp.style = m_style;
			sp.width = rcWnd.right - rcWnd.left;
			sp.height = rcWnd.bottom - rcWnd.top;
			sp.xPos = rcWnd.left;
			sp.yPos = rcWnd.top;
			sp.resizable = m_resizable;
			sp.maximized = m_maximized;
			m_style &= ~(WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
			m_style |= WS_POPUP | WS_VISIBLE;
			m_resizable = false;
			m_maximized = false;
			SetWindowLong(m_hWnd, GWL_STYLE, m_style);
			MONITORINFO mi{};
			mi.cbSize = sizeof(mi);
			GetMonitorInfo(MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTONEAREST), &mi);
			SetWindowPos(m_hWnd, NULL, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			m_fullscreen = true;
		}
		else if (!fullscreen && m_fullscreen) {
			m_style = sp.style;
			m_resizable = sp.resizable;
			SetWindowLong(m_hWnd, GWL_STYLE, m_style | WS_VISIBLE);
			SetWindowPos(m_hWnd, NULL, sp.xPos, sp.yPos, sp.width, sp.height, SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			m_fullscreen = false;
			if (sp.maximized) {
				setMaximized(true);
			}
		}
	}

	void Win32Window::setMaximized(bool maximized) {
		if (!m_fullscreen) {
			if (maximized && !m_maximized) {
				ShowWindow(m_hWnd, SW_MAXIMIZE);
			}
			else if (!maximized && m_maximized) {
				ShowWindow(m_hWnd, SW_RESTORE);
			}
		}
	}

	void Win32Window::setMinimized(bool minimized) {
		if (minimized && !m_minimized) {
			ShowWindow(m_hWnd, SW_MINIMIZE);
		}
		else if (!minimized && m_minimized) {
			ShowWindow(m_hWnd, SW_RESTORE);
		}
	}

	void Win32Window::setVisible(bool visible) {
		if (visible && !m_visible) {
			ShowWindow(m_hWnd, SW_SHOW);
			m_visible = true;
		} else if (!visible && m_visible) {
			setFocused(false);
			ShowWindow(m_hWnd, SW_HIDE);
			m_visible = false;
		}
	}

	void Win32Window::setFocused(bool focused) {
		if (focused && !m_focused) {
			setVisible();
			SetFocus(m_hWnd);
			SetForegroundWindow(m_hWnd);
			m_focused = true;
		}
		else if (!focused && m_focused) {
			SetFocus(NULL);
			m_focused = false;
		}
	}

	HINSTANCE Win32Window::getHInstance() const {
		return m_hInstance;
	}

	HWND Win32Window::getHWND() const {
		return m_hWnd;
	}

	HDC Win32Window::getHDC() const {
		return m_hDC;
	}

	std::string Win32Window::getTitle() const {
		return m_title;
	}

	int Win32Window::getWidth() const {
		return m_width;
	}

	int Win32Window::getHeight() const {
		return m_height;
	}

	float Win32Window::getAspectRatio() const {
		return m_aspectRatio;
	}

	int Win32Window::getXPos() const {
		return m_xPos;
	}

	int Win32Window::getYPos() const {
		return m_yPos;
	}

	bool Win32Window::isResizable() const {
		return m_resizable;
	}

	bool Win32Window::isFullscreen() const {
		return m_fullscreen;
	}

	bool Win32Window::isMaximized() const {
		return m_maximized;
	}

	bool Win32Window::isMinimized() const {
		return m_minimized;
	}

	bool Win32Window::isVisible() const {
		return m_visible;
	}

	bool Win32Window::isFocused() const {
		return m_focused;
	}

	int Win32Window::getExitCode() const {
		return m_exitCode;
	}

	bool Win32Window::keyPressed(KEY key) const {
		return m_keyPressed[(int)key];
	}

	bool Win32Window::buttonPressed(BUTTON button) const {
		return m_buttonPressed[(int)button];
	}

	LRESULT Win32Window::msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
				IF_EVENT_CALLBACK(m_eventCallback(WindowCloseEvent()))
				break;
			case WM_SIZE:
				m_width = (int)LOWORD(lParam);
				m_height = (int)HIWORD(lParam);
				m_aspectRatio = (float)m_height / (float)m_width;
				if (wParam == SIZE_MAXIMIZED) {
					m_maximized = true;
					IF_EVENT_CALLBACK(m_eventCallback(WindowMaximizeEvent()))
				}
				else if (wParam == SIZE_RESTORED && m_maximized) {
					m_maximized = false;
					IF_EVENT_CALLBACK(m_eventCallback(WindowRestoreEvent(EventType::WindowMaximize)))
				}
				else if (wParam == SIZE_MINIMIZED) {
					m_minimized = true;
					IF_EVENT_CALLBACK(m_eventCallback(WindowMinimizeEvent()))
				}
				else if (wParam == SIZE_RESTORED && m_minimized) {
					m_minimized = false;
					IF_EVENT_CALLBACK(m_eventCallback(WindowRestoreEvent(EventType::WindowMinimize)))
				}
				IF_EVENT_CALLBACK(m_eventCallback(WindowResizeEvent(m_width, m_height)))
				break;
			case WM_MOVE:
				m_xPos = (int)LOWORD(lParam);
				m_yPos = (int)HIWORD(lParam);
				IF_EVENT_CALLBACK(m_eventCallback(WindowMoveEvent(m_xPos, m_yPos)))
				break;
			case WM_SETFOCUS:
				m_focused = true;
				IF_EVENT_CALLBACK(m_eventCallback(WindowFocusEvent()))
				break;
			case WM_KILLFOCUS:
				m_focused = false;
				IF_EVENT_CALLBACK(m_eventCallback(WindowLostFocusEvent()))
				break;
			case WM_LBUTTONDOWN:
				m_buttonPressed[(int)BUTTON::LEFT] = true;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonPressEvent((int)BUTTON::LEFT)))
				break;
			case WM_RBUTTONDOWN:
				m_buttonPressed[(int)BUTTON::RIGHT] = true;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonPressEvent((int)BUTTON::RIGHT)))
				break;
			case WM_MBUTTONDOWN:
				m_buttonPressed[(int)BUTTON::MIDDLE] = true;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonPressEvent((int)BUTTON::MIDDLE)))
				break;
			case WM_XBUTTONDOWN:
				if (HIWORD(wParam) == XBUTTON1) {
					m_buttonPressed[(int)BUTTON::BUTTON_4] = true;
					IF_EVENT_CALLBACK(m_eventCallback(MouseButtonPressEvent((int)BUTTON::BUTTON_4)))
				}
				else if (HIWORD(wParam) == XBUTTON2) {
					m_buttonPressed[(int)BUTTON::BUTTON_5] = true;
					IF_EVENT_CALLBACK(m_eventCallback(MouseButtonPressEvent((int)BUTTON::BUTTON_5)))
				}
				break;
			case WM_LBUTTONUP:
				m_buttonPressed[(int)BUTTON::LEFT] = false;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonReleaseEvent((int)BUTTON::LEFT)))
				break;
			case WM_RBUTTONUP:
				m_buttonPressed[(int)BUTTON::RIGHT] = false;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonReleaseEvent((int)BUTTON::RIGHT)))
				break;
			case WM_MBUTTONUP:
				m_buttonPressed[(int)BUTTON::MIDDLE] = false;
				IF_EVENT_CALLBACK(m_eventCallback(MouseButtonReleaseEvent((int)BUTTON::MIDDLE)))
				break;
			case WM_XBUTTONUP:
				if (HIWORD(wParam) == XBUTTON1) {
					m_buttonPressed[(int)BUTTON::BUTTON_4] = false;
					IF_EVENT_CALLBACK(m_eventCallback(MouseButtonReleaseEvent((int)BUTTON::BUTTON_4)))
				}
				else if (HIWORD(wParam) == XBUTTON2) {
					m_buttonPressed[(int)BUTTON::BUTTON_5] = false;
					IF_EVENT_CALLBACK(m_eventCallback(MouseButtonReleaseEvent((int)BUTTON::BUTTON_5)))
				}
				break;
			case WM_MOUSEMOVE:
				IF_EVENT_CALLBACK(m_eventCallback(MouseMoveEvent((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam))))
				break;
			case WM_MOUSEWHEEL:
				IF_EVENT_CALLBACK(m_eventCallback(MouseScrollEvent((float)GET_WHEEL_DELTA_WPARAM(wParam))))
				break;
			case WM_KEYDOWN:
				m_keyPressed[(int)wParam] = true;
				IF_EVENT_CALLBACK(m_eventCallback(KeyPressEvent((int)wParam, LOWORD(lParam))))
				break;
			case WM_KEYUP:
				m_keyPressed[(int)wParam] = false;
				IF_EVENT_CALLBACK(m_eventCallback(KeyReleaseEvent((int)wParam)))
				break;
			default:
				break;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	LRESULT CALLBACK Win32Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		Win32Window *window = (Win32Window *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!window) {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else {
			return window->msgProc(hWnd, message, wParam, lParam);
		}
	}
}

#undef IF_EVENT_CALLBACK