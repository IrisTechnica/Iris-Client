#pragma once

#include <array>
#include <memory>

namespace win32
{
	class BaseWindow;
	typedef std::shared_ptr<BaseWindow> BaseWindowPtr, ParentWindowPtr;
};

//type of raw input
class key
{
public:
	key();
	~key();


	void Initialize(win32::BaseWindowPtr window);

	inline bool GetTrigger(unsigned char key)
	{
		return trigger_[key];
	};
	inline bool GetPress(unsigned char key)
	{
		return press_[key];
	};
	inline bool GetRelease(unsigned char key)
	{
		return release_[key];
	};
	inline bool GetMouseTrigger(unsigned int btn)
	{
		return mousetrigger_[btn];
	};
	inline bool GetMousePress(unsigned int btn)
	{
		return mousepress_[btn];
	};
	inline bool GetMouseRelease(unsigned int btn)
	{
		return mouserelease_[btn];
	};
	inline int2 GetMousePos()
	{
		return mousepos_;
	};

	inline int2 GetDeltaPos()
	{
		return deltapos_;
	}

	inline short GetMouseWheel()
	{
		return mousewheel_;
	}

	void UpdateBuffer();

private:
	void InternalInitialize(win32::window_handle window);
	void InternalUpdateRawInput(HRAWINPUT input_handle);

private:

	std::array<bool,UCHAR_MAX> trigger_;
	std::array<bool,UCHAR_MAX> trigger_next_;
	std::array<bool,UCHAR_MAX> press_;
	std::array<bool,UCHAR_MAX> release_;

	std::array<bool, 5> mousetrigger_;
	std::array<bool, 5> mousepress_;
	std::array<bool, 5> mouserelease_;

	int2 mousepos_;
	int2 deltapos_;
	short mousewheel_;
};

typedef std::shared_ptr<key> key_ptr;