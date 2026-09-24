#include <Window/Window.h>

#include <Helpers/MonitorHelpers.h>

#ifdef KOORI_WINDOWS

static std::unordered_map<HWND, Window*> m_Windows;

#endif //KOORI_WINDOWS

Window::Window()
{
	KooriCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowXPos, m_WindowYPos);
}

Window::Window(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos)
	:m_WindowWidth(_width), m_WindowHeight(_height), m_WindowXPos(_xPos), m_WindowYPos(_yPos)
{
	KooriCreateWindow(m_WindowWidth, m_WindowHeight, m_WindowXPos, m_WindowYPos);
}

void Window::KooriCreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos)
{
#ifdef KOORI_WINDOWS
	WIN_CreateWindow(_width, _height, _xPos, _yPos);
#endif //KOORI_WINDOWS

	ShowWindow();
}

void Window::KooriDestroyWindow()
{
}

bool Window::WindowShouldClose()
{
	return m_WindowShouldClose;
}

void Window::UpdateWindow()
{
#ifdef KOORI_WINDOWS
	WIN_UpdateWindow();
#endif //KOORI_WINDOWS
}

void Window::ShowWindow()
{
#ifdef KOORI_WINDOWS
	::ShowWindow(m_WindowHandle, SW_SHOW);
#endif //KOORI_WINDOWS
}

void Window::HideWindow()
{
#ifdef KOORI_WINDOWS
	::ShowWindow(m_WindowHandle, SW_HIDE);
#endif //KOORI_WINDOWS
}

bool Window::SetFullScreen(bool _fullScreen)
{
	m_IsFullScreen = _fullScreen;
	ToogleFullScreen();
	return m_IsFullScreen;
}

void Window::ToogleFullScreen()
{
	m_IsFullScreen = !m_IsFullScreen;

	if (m_IsFullScreen)
	{
#ifdef KOORI_WINDOWS
		WIN_EnableFullScreen();
#endif //KOORI_WINDOWS
	}
	else
	{
#ifdef KOORI_WINDOWS
		WIN_DisableFullScreen();
#endif //KOORI_WINDOWS
	}
}

#ifdef KOORI_WINDOWS

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* currentWindow = m_Windows[hwnd];

	switch (uMsg)
	{
	case WM_CLOSE:
	{
		currentWindow->m_WindowShouldClose = true;
		::DestroyWindow(hwnd);
		return 0;
	}
	case WM_DESTROY:
	{
		::PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:
	{
		currentWindow->m_WindowWidth = static_cast<uint32_t>(LOWORD(lParam));
		currentWindow->m_WindowHeight = static_cast<uint32_t>(HIWORD(lParam));

		if(currentWindow->m_ResizeCallback)
			currentWindow->m_ResizeCallback(currentWindow->m_WindowWidth, currentWindow->m_WindowHeight);
	}
	break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::WIN_CreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos)
{
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = m_ClassName;

	ATOM atom = RegisterClass(&wc);
	DWORD err;

	// KOORI_ASSERT

	RECT windowRect = { _xPos, _yPos, _width, _height };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;

	m_WindowHandle = CreateWindowEx(0, m_ClassName, m_WindowName,
		WS_OVERLAPPEDWINDOW, _xPos, _yPos, _width, _height,
		NULL, NULL, GetModuleHandle(0), NULL);

	// KOORI_ASSERT

	m_Windows[m_WindowHandle] = this;

	return;
}

void Window::WIN_UpdateWindow()
{
	MSG msg = { 0 };
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window::WIN_EnableFullScreen()
{
	// Define the new window style without borders or controls
	const UINT windowStyle = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	::SetWindowLongW(m_WindowHandle, GWL_STYLE, windowStyle);

	// Get current monitor info
	const HMONITOR hMonitor = MonitorFromWindow(m_WindowHandle, MONITOR_DEFAULTTONEAREST);
	MONITORINFOEX monitorInfo = {};
	monitorInfo.cbSize = sizeof(MONITORINFOEX);
	::GetMonitorInfo(hMonitor, &monitorInfo);

	// Resize and reposition the window
	int cx = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	int cy = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	::SetWindowPos(m_WindowHandle, HWND_TOP, monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, cx, cy, SWP_FRAMECHANGED | SWP_NOACTIVATE);

	// Maximize the window
	::ShowWindow(m_WindowHandle, SW_MAXIMIZE);
}

void Window::WIN_DisableFullScreen()
{
	// Restore standard window style
	::SetWindowLong(m_WindowHandle, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	// Restore window position and size
	::SetWindowPos(m_WindowHandle, HWND_TOP, m_WindowXPos, m_WindowYPos, m_WindowWidth, m_WindowHeight, SWP_FRAMECHANGED | SWP_NOACTIVATE);

	// Show the window in normal mode
	::ShowWindow(m_WindowHandle, SW_NORMAL);
}

#endif //KOORI_WINDOWS