#include "SceneManager.h"
#include <stx/logger.hpp>
#ifdef _DEBUG
#include <stx/win32/windowmanager.h>
#include <stx/win32/key.hpp>
#endif

namespace manager
{
	BaseScene::BaseScene(tstring name)
		: name(name)
	{
#ifdef _DEBUG
		on_next += [&]()
		{
			stx::logger::Debug("Scene Changed %s -> %s", this->name, this->next_scene->name);
		};
#endif
	}

	BaseScenePtr BaseScene::next()
	{
		if (next_scene)
		{
			on_next();
			next_scene->initialize();
			return next_scene;
		}

		static const auto this_ptr = this->shared_from_this();
		return this_ptr;
	}

	SceneManager::~SceneManager()
	{
		this->WaitTask();
	}

	void SceneManager::Start(BaseScenePtr boot_scene, bool step)
	{
		this->current_scene = std::make_shared<SceneBootStrap>(std::move(boot_scene));

#ifdef _DEBUG
		this->step_mode = step;
		if (step_mode)
			stx::logger::Debug("SceneManager is StepMode");
#endif

		this->scene_thread = std::thread([&]() 
		{
			while (this->current_scene && !this->is_terminated)
			{
				auto window = win32::WindowManager::GetInstance()->GetCurrentWindow();
				if (!window)continue;

				auto key = window->GetKey();
				if (!key)continue;

#ifdef _DEBUG
				if (step_mode)
				{
					if (!key->GetTrigger(VK_F8)) {
						continue;
					};
					stx::logger::Debug("step");
				}
#endif
				this->current_scene->process();
				this->current_scene = std::move(this->current_scene->next());

				key->UpdateBuffer();
			}
		});
	}

	void SceneManager::Terminate()
	{
		is_terminated = true;
	}

	void SceneManager::WaitTask()
	{
		if (this->scene_thread.joinable())
			this->scene_thread.join();
	}
}