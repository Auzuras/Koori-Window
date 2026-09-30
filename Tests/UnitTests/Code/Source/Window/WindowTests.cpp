#include <gtest/gtest.h>

#include <Window/Window.h>
#include <Helpers/MonitorHelpers.h>

class WindowTest : public testing::Test {};

TEST(WindowTest, Test)
{
	const uint16_t windowWidth = 1920;
	const uint16_t windowHeight = 1080;

	const uint16_t centeredXPos = KOORI_PRIMARY_MONITOR_CENTER_X - windowWidth * 0.5;
	const uint16_t centeredYPos = KOORI_PRIMARY_MONITOR_CENTER_Y - windowHeight * 0.5;

	Window window(windowWidth, windowHeight, "Custom window", centeredXPos, centeredYPos);

	while (!window.WindowShouldClose())
	{
		window.UpdateWindow();
	}
}	
