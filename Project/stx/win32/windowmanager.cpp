#include "windowmanager.h"
#include "window.h"
#include <queue>

namespace win32
{

	WindowManager::WindowManager()
	{
	}


	WindowManager::~WindowManager()
	{
	}

	void WindowManager::Regist(tstring name, BaseWindowPtr window)
	{
		window_map_.insert({ stx::tstring_symbol(name),window });
	}

	BaseWindowPtr WindowManager::operator[](tstring name)
	{
		BaseWindowPtr window;
		stx::tstring_symbol hash = name;
		if (window_map_.find(hash) != window_map_.end())window = window_map_[hash];
		return window;
	}

	BaseWindowPtr WindowManager::GetFirst()
	{
		BaseWindowPtr window;
		if(!window_map_.empty())window = window_map_.begin()->second;
		return window;
	}

	BaseWindowPtr WindowManager::GetCurrentWindow()
	{
		BaseWindowPtr window;
		if (!current_window_name_.empty())window = operator[](current_window_name_);
		return window;
	}

	void WindowManager::SetCurrentWindow(tstring name)
	{
		current_window_name_ = name;
	}

	void WindowManager::SetCurrentWindow(BaseWindowPtr window)
	{
		current_window_name_ = window->GetName();
	}

	void WindowManager::Run()
	{
		std::queue<decltype(window_map_)::iterator> it_que;
		Apply(WindowManagerHook::Initialize);
		// 何らかのWindowが生きている限りはループする
		while (!window_map_.empty())
		{

			for (auto& window_pair : window_map_)
			{
				auto window = window_pair.second;
				if (window->ProcessMessage() != 0 || !window->isLive())
					it_que.push(window_map_.find(window_pair.first));
			}

			Apply(WindowManagerHook::BeforeRenderEntire);
			for (auto& window_pair : window_map_)
			{
				auto window = window_pair.second;
				if (!window->isLive())continue;
				this->SetCurrentWindow(window);
				Apply(WindowManagerHook::BeforeRender);
				window->ProcessHolderWrapper();
				Apply(WindowManagerHook::AfterRender);
				Apply(WindowManagerHook::AfterRenderFinalize);
			}
			Apply(WindowManagerHook::AfterRenderEntire);
			// Windowが死んだらマップから削除する
			while (!it_que.empty())
			{
				auto it = it_que.front();
				it_que.pop();
				it->second->Destroy();
				it->second.reset();
				window_map_.erase(it);
			}
		}
		Apply(WindowManagerHook::Finalize);
	}

	void WindowManager::Apply(WindowManagerHook hook)
	{
		if (hook_map_.find(hook) != hook_map_.end())hook_map_[hook]();
	}

	void WindowManager::AddHook(WindowManagerHook hook, std::function<void()> function)
	{
		hook_map_.insert(std::make_pair(hook, function));
	}

};