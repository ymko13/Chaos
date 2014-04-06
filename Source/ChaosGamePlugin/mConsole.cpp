#include "ChaosGamePluginPCH.h"
#include "mConsole.h"

namespace mDraw
{
	static const void mDraw::SetConsole(IVConsoleManager* console)
	{
		mDraw::console = console;
	}
	static const void mDraw::SetVisible(bool t)
	{
		mDraw::console->Show(t);
	}
	static const void mDraw::ConsolePrintLine(VString s)
	{
		mDraw::console->OutputTextLine(s);
	}
}