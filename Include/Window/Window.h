#ifndef KOORI_WINDOW_WINDOW_H
#define KOORI_WINDOW_WINDOW_H

#include <cstdint>
#include <unordered_map>
#include <functional>

#ifdef KOORI_WINDOWS

#include <Windows.h>

#endif //KOORI_WINDOWS

class Window
{
public:

	using KooriResizeCallaback = std::function<void(uint32_t _width, uint32_t _height)>;

	Window();
	Window(uint16_t _width, uint16_t _height, uint16_t _xPos = 0u, uint16_t _yPos = 0u);

	void KooriCreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos = 0u, uint16_t _yPos = 0u);

	void KooriDestroyWindow();

	bool WindowShouldClose();

	void UpdateWindow();

	void ShowWindow();

	void HideWindow();

	bool IsFullScreen() const { return m_IsFullScreen; }
	bool SetFullScreen(bool _fullScreen);
	void ToogleFullScreen();

	uint16_t GetWidth() const { return m_WindowWidth; }
	uint16_t GetHeight() const { return m_WindowHeight; }

	uint16_t GetXPos() const { return m_WindowXPos; }
	uint16_t GetYPos() const { return m_WindowYPos; }

	void SetResizeCallback(KooriResizeCallaback _callback)
	{
		m_ResizeCallback = std::move(_callback);
	}

#ifdef KOORI_WINDOWS

	HWND GetHandle() const { return m_WindowHandle; }

#endif //KOORI_WINDOWS

private:

#ifdef KOORI_WINDOWS

	void WIN_CreateWindow(uint16_t _width, uint16_t _height, uint16_t _xPos, uint16_t _yPos);

	void WIN_UpdateWindow();

	void WIN_EnableFullScreen();
	void WIN_DisableFullScreen();

#endif //KOORI_WINDOWS

private:

	uint16_t m_WindowWidth = 1280u;
	uint16_t m_WindowHeight = 720u;

	uint16_t m_WindowXPos = 0u;
	uint16_t m_WindowYPos = 0u;

	bool m_IsFullScreen = false;
	bool m_WindowShouldClose = false;

	KooriResizeCallaback m_ResizeCallback = nullptr;

#ifdef KOORI_WINDOWS

	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	const char* m_ClassName = "Koori Window";
	const char* m_WindowName = "Koori Window";

	HWND m_WindowHandle = NULL;

#endif //KOORI_WINDOWS
};

#endif //KOORI_WINDOW_WINDOW_H