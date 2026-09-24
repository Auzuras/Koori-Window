#include <gtest/gtest.h>

#include <Window/Window.h>
#include <Helpers/MonitorHelpers.h>

class WindowTest : public testing::Test {};

TEST(WindowTest, Test)
{
	Window window(1920, 1080);

	while (!window.WindowShouldClose())
	{
		window.UpdateWindow();
	}
}	
