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
	WIN_CreateWindow(_width, _height, _xPos, _yPos);
}

bool Window::WindowShouldClose()
{
	return m_WindowShouldClose || !m_WindowHandle ? true : false;
}

void Window::UpdateWindow()
{
	WIN_UpdateWindow();
}

void Window::KooriShowWindow()
{
	ShowWindow(m_WindowHandle, SW_SHOW);
}

void Window::KooriHideWindow()
{
	ShowWindow(m_WindowHandle, SW_HIDE);
}

#ifdef KOORI_WINDOWS

void Window::WIN_CreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos)
{
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(0);
	wc.lpszClassName = m_ClassName;

	ATOM atom = RegisterClass(&wc);
	DWORD err;

	const uint16_t correctedX = _xPos + KOORI_PRIMARY_MONITOR_CENTER_X - _width * 0.5;
	const uint16_t correctedY = _yPos + KOORI_PRIMARY_MONITOR_CENTER_Y - _height * 0.5;

	m_WindowHandle = CreateWindowEx(0, m_ClassName, m_WindowName, 
		WS_OVERLAPPEDWINDOW, correctedX, correctedY, _width, _height,
		NULL, NULL, GetModuleHandle(0), NULL);

	m_Windows[m_WindowHandle] = this;

	return;
}

void Window::WIN_UpdateWindow()
{
	if (!m_WindowHandle)
		return;

	MSG msg = { 0 };
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		m_Windows[hwnd]->m_WindowShouldClose = true;
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

#endif //KOORI_WINDOWS