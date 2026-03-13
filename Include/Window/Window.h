#ifndef KOORI_WINDOW_WINDOW_H
#define KOORI_WINDOW_WINDOW_H

#include <cstdint>
#include <unordered_map>

#ifdef KOORI_WINDOWS

#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif //KOORI_WINDOWS

class Window
{
public:

	Window();
	Window(uint16_t _width, uint16_t _height, uint16_t _xPos = 0u, uint16_t _yPos = 0u);

	void KooriCreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos = 0u, uint16_t _yPos = 0u);

	bool WindowShouldClose();

	void UpdateWindow();

	void KooriShowWindow();

	void KooriHideWindow();

private:

#ifdef KOORI_WINDOWS

	void WIN_CreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos);
	void WIN_UpdateWindow();

#endif //KOORI_WINDOWS

private:

	uint16_t m_WindowWidth = 1920u;
	uint16_t m_WindowHeight = 1080u;

	uint16_t m_WindowXPos = 0u;
	uint16_t m_WindowYPos = 0u;

	bool m_WindowShouldClose = false;

#ifdef KOORI_WINDOWS

	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	const char* m_ClassName = "Koori Window";
	const char* m_WindowName = "Koori Window";

	HWND m_WindowHandle = NULL;

#endif //KOORI_WINDOWS
};

#endif //KOORI_WINDOW_WINDOW_H