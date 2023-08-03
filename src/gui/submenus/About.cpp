#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::About()
{
	GUI::Title("About");
	GUI::Break("Author: NIGHTMARE");
	if (GUI::Option("Visit GitHub Repository", ""))
	{
		system("start https://github.com/golummodz/G-mod-menu");
	}
	if (GUI::Option("Unload Cheat", ""))
	{
		// Remove console window
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		FreeConsole();

		// Stop MainFiber
		core::c_running = false;
	}
}