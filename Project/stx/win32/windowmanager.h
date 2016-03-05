#pragma once
#include "../singleton.hpp"
#include <memory>
#include <unordered_map>
#include "../tstring_symbol.hpp"

namespace win32
{
	enum class WindowManagerHook
	{
		BeforeRenderEntire,
		AfterRenderEntire,
		BeforeRender,
		AfterRender,
		AfterRenderFinalize,
		Finalize,
		Initialize
	};

	class BaseWindow;
	typedef std::shared_ptr<BaseWindow> BaseWindowPtr;

	class WindowManager :
		public stx::refered_singleton<WindowManager>
	{
	private:
		std::unordered_map<stx::tstring_symbol, BaseWindowPtr> window_map_;
		std::unordered_map<WindowManagerHook, std::function<void()>> hook_map_;
		tstring current_window_name_;

		void Apply(WindowManagerHook hook);
	public:
		WindowManager();
		virtual ~WindowManager();

		void Regist(tstring name, BaseWindowPtr window);
		BaseWindowPtr operator[](tstring name);
		BaseWindowPtr GetFirst(); // とりあえず何でもいいからウィンドウが欲しいとき用

		BaseWindowPtr GetCurrentWindow();
		template<class _Ptr>
		_Ptr GetCurrentWindow()
		{
			BaseWindowPtr window;
			if (!current_window_name_.empty())window = operator[](current_window_name_);
			return std::dynamic_pointer_cast<_Ptr::element_type>(window);
		}

		void SetCurrentWindow(tstring name);
		void SetCurrentWindow(BaseWindowPtr window);

		void Run();

		void AddHook(WindowManagerHook hook, std::function<void()> function);

	};

};