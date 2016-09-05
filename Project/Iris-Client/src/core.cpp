#include "core.h"
#include <stx/logger.hpp>
#include <stx/config_file.hpp>
#include <stx/win32/windowmanager.h>
#include "SceneManager.h"

Core::Core()
{
}


Core::~Core()
{
}

void Core::Run()
{
	this->Initialize();
	auto scene_manager = manager::SceneManager::GetInstance();
	auto window_manager = win32::WindowManager::GetInstance();

	scene_manager->Start(std::make_shared<manager::BaseScene>("test"), true);

	window_manager->Run();
	scene_manager->WaitTask();
}

void Core::Initialize()
{
	stx::filesystem::current_path_adjustment();

	{
		stx::logger::scope_message<stx::logger::begin_end> msg(_T("Configuration"));

		stx::config_file::GetInstance()->set_directory("./config/");
		stx::config_file::GetInstance()->load("core.config");
		stx::config_file::GetRefInstance()[_T("name")] << _T("Iris Engine");

	}

	this->window = MainWindow::Create();

}
