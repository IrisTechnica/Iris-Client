#pragma once
#include <memory>
#include <stx/win32/window.h>
#include <Graphics/src/interface/framemodel.h>

class MainWindow;
typedef std::shared_ptr<MainWindow> MainWindowPtr;

class MainWindow : 
	public win32::BaseWindow,
	public interface::framemodel
{
public:
	MainWindow(tstring name) : BaseWindow(name) {};

	void Update() {};
	void Draw() {};
public:
	static MainWindowPtr Create();
};