#pragma once
#include <stx/delegate.hpp>
#include <memory>
#include <stx/unicode.hpp>
#include <stx/singleton.hpp>
#include <thread>
#include <atomic>

namespace manager
{
	class BaseScene;
	using BaseScenePtr = std::shared_ptr<BaseScene>;

	class BaseScene : public std::enable_shared_from_this<BaseScene>
	{
	public:
		using this_type = BaseScene;

	public:
		BaseScene() = delete;
		BaseScene(tstring name);

		virtual void process() {};
		BaseScenePtr next();

	protected:
		virtual void initialize() {};

	protected:
		stx::delegate on_next;

	protected:
		BaseScenePtr next_scene;
		tstring name;
	};

	namespace
	{
		class SceneBootStrap : public BaseScene
		{
		public:
			SceneBootStrap(BaseScenePtr boot_scene) 
				: BaseScene("scene boot strap")
			{
				next_scene = boot_scene;
			};
		private:
			BaseScenePtr boot_scene;
		};
	}
	class SceneManager : public stx::singleton<SceneManager>
	{
	public:
		~SceneManager();
		BaseScenePtr GetCurrentScene() const { return current_scene; }
		void Start(BaseScenePtr boot_scene, bool step = false);
		void Terminate();
		void WaitTask();
	private:
		BaseScenePtr current_scene;
		std::thread scene_thread;
		std::atomic<bool> is_terminated = false;
		bool step_mode = false;
	};
};