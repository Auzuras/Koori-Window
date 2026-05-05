#include <gtest/gtest.h>

#include <Window/Window.h>
#include <Helpers/MonitorHelpers.h>

class WindowTest : public testing::Test {};

TEST(WindowTest, Test)
{
	Window window(960, 540);

	while (!window.WindowShouldClose())
	{
		window.UpdateWindow();
	}
}
