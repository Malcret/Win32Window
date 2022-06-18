#include "W32W/Win32Window.hpp"
#include <windowsx.h>
#include "W32W/Utility.hpp"

namespace W32W {
	Win32Window::Win32Window()
		: m_CLASS_NAME(L""), m_hInstance(nullptr), m_hWnd(nullptr), m_hDC(nullptr), m_title(""), m_width(0), m_height(0), m_aspectRatio(0), m_shouldClose(0), m_exitCode(0) { }

	Win32Window::Win32Window(const std::string &title, int width, int height)
		: m_CLASS_NAME(L"WindowClass"), m_hInstance(GetModuleHandle(nullptr)), m_title(title), m_width(width), m_height(height), m_aspectRatio((float)m_height / (float)m_width), m_shouldClose(false), m_exitCode(0) {
		WNDCLASS wndClass {};
		wndClass.lpszClassName = m_CLASS_NAME;                           // Class name
		wndClass.hInstance = m_hInstance;                                // Instance
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);                    // Icon
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                  // Cursor
		wndClass.lpfnWndProc = windowProc;                               // Callback
		wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // Background color
		wndClass.style = CS_OWNDC;                                       // Redraw on move, own DC for window
		wndClass.cbClsExtra = sizeof(int);
		RegisterClass(&wndClass);

		// Set the window styles
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

		const std::wstring wTITLE(m_title.begin(), m_title.end());

		// Create the window
		if (!(m_hWnd = CreateWindowEx(
			0,                // Additional styles
			m_CLASS_NAME,     // Class name
			&wTITLE[0],       // Title
			style,            // Styles
			CW_USEDEFAULT,    // X position
			CW_USEDEFAULT,    // Y position
			m_width,          // Width
			m_height,         // Height
			NULL,             // Parent window
			NULL,             // Menu
			m_hInstance,      // Instance
			NULL              // Additional application data
			))) {
			W32W_ASSERT(false, "Failed to create window");
		}

		if (!(m_hDC = GetDC(m_hWnd))) {
			W32W_ASSERT(false, "Failed to get device context");
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

	void Win32Window::show() {
		ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);
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
		SetWindowText(m_hWnd, &wTitle[0]);
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

	int Win32Window::getExitCode() const {
		return m_exitCode;
	}

	LRESULT Win32Window::msgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
			case WM_CLOSE:
				m_eventCallback(WindowCloseEvent());
				break;
			case WM_SIZE:
				m_width = LOWORD(lParam);
				m_height = HIWORD(lParam);
				m_aspectRatio = (float)m_height / (float)m_width;
				m_eventCallback(WindowResizeEvent(m_width, m_height));
				break;
			case WM_LBUTTONDOWN:
				m_eventCallback(MouseButtonPressedEvent(W32W_MOUSE_BUTTON_LEFT));
				break;
			case WM_RBUTTONDOWN:
				m_eventCallback(MouseButtonPressedEvent(W32W_MOUSE_BUTTON_RIGHT));
				break;
			case WM_MBUTTONDOWN:
				m_eventCallback(MouseButtonPressedEvent(W32W_MOUSE_BUTTON_MIDDLE));
				break;
			case WM_XBUTTONDOWN:
				if (HIWORD(wParam) == XBUTTON1) {
					m_eventCallback(MouseButtonPressedEvent(W32W_MOUSE_BUTTON_4));
				}
				else if (HIWORD(wParam) == XBUTTON2) {
					m_eventCallback(MouseButtonPressedEvent(W32W_MOUSE_BUTTON_5));
				}
				break;
			case WM_LBUTTONUP:
				m_eventCallback(MouseButtonReleasedEvent(W32W_MOUSE_BUTTON_LEFT));
				break;
			case WM_RBUTTONUP:
				m_eventCallback(MouseButtonReleasedEvent(W32W_MOUSE_BUTTON_RIGHT));
				break;
			case WM_MBUTTONUP:
				m_eventCallback(MouseButtonReleasedEvent(W32W_MOUSE_BUTTON_MIDDLE));
				break;
			case WM_XBUTTONUP:
				if (HIWORD(wParam) == XBUTTON1) {
					m_eventCallback(MouseButtonReleasedEvent(W32W_MOUSE_BUTTON_4));
				}
				else if (HIWORD(wParam) == XBUTTON2) {
					m_eventCallback(MouseButtonReleasedEvent(W32W_MOUSE_BUTTON_5));
				}
				break;
			case WM_MOUSEMOVE:
				m_eventCallback(MouseMovedEvent((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam)));
				break;
			case WM_MOUSEWHEEL:
				m_eventCallback(MouseScrolledEvent((float)GET_WHEEL_DELTA_WPARAM(wParam)));
				break;
			case WM_KEYDOWN:
				m_eventCallback(KeyPressedEvent((int)wParam, LOWORD(lParam)));
				break;
			case WM_KEYUP:
				m_eventCallback(KeyReleasedEvent((int)wParam));
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