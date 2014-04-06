#pragma once
namespace mDraw
{
	static IVConsoleManager* console;
	static const void SetConsole(IVConsoleManager* console);
	static const void SetVisible(bool t);
	static const void ConsolePrintLine(VString s);
}

