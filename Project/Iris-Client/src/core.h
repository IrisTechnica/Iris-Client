#pragma once
#include <stx/win32/window.h>
#include "mainwindow.h"

class Core
{
private:

	MainWindowPtr window;

public:
	Core();
	~Core();

	void Run();

private:
	void Initialize();
};

