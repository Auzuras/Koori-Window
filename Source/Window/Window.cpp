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
	WIN_UpdateWindow();
}

void Window::ShowWindow()
{
	::ShowWindow(m_WindowHandle, SW_SHOW);
}

void Window::HideWindow()
{
	::ShowWindow(m_WindowHandle, SW_HIDE);
}

#ifdef KOORI_WINDOWS

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* currentWindow = m_Windows[hwnd];

	switch (uMsg)
	{
	case WM_CLOSE:
		currentWindow->m_WindowShouldClose = true;
		::DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
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

	RECT windowRect = { 0, 0, _width, _height };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	const uint16_t correctedX = _xPos + KOORI_PRIMARY_MONITOR_CENTER_X - _width * 0.5;
	const uint16_t correctedY = _yPos + KOORI_PRIMARY_MONITOR_CENTER_Y - _height * 0.5;

	_width = windowRect.right - windowRect.left;
	_height = windowRect.bottom - windowRect.top;

	m_WindowHandle = CreateWindowEx(0, m_ClassName, m_WindowName,
		WS_OVERLAPPEDWINDOW, correctedX, correctedY, _width, _height,
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

#endif //KOORI_WINDOWS