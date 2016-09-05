#include "mainwindow.h"

#include <stx/config_file.hpp>
#include <stx/logger.hpp>

MainWindowPtr MainWindow::Create()
{
	stx::logger::scope_message<stx::logger::begin_end> msg(_T("Create Window"));

	auto& config = stx::config_file::GetRefInstance();
	auto class_name = config["class_name"].get_default<tstring>("iris");
	auto window_name = config["window_name"].get_default<tstring>("iris");
	auto window_size = int4({ CW_USEDEFAULT , CW_USEDEFAULT, 1280, 720 });

	return win32::FactoryWindow::CreateNormalWindow<MainWindowPtr>(class_name, window_name, window_size, nullptr);
}
