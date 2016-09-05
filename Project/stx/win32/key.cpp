#include "../range.hpp"
#include "window.h"
#include "key.hpp"
#include "../typedef.h"

key::key()
{
	trigger_.fill(false);
	trigger_next_.fill(false);
	press_.fill(false);
	release_.fill(false);
	mousetrigger_.fill(false);
	mousepress_.fill(false);
	mouserelease_.fill(false);
}


key::~key()
{
}


void key::Initialize(win32::BaseWindowPtr window)
{
	//hook initialize
	window
		->AddMessageProcMap(
	{
		WM_CLASSCREATE,
		[this](HWND hWnd,WPARAM,LPARAM)->LRESULT
	{
		InternalInitialize(hWnd);
		return 0;
	}
	})
		->AddMessageProcMap(
	{
		WM_INPUT,
		[this](HWND,WPARAM,LPARAM lParam)->LRESULT
	{
		InternalUpdateRawInput((HRAWINPUT)lParam);
		return 0;
	}
	})
		->Post(WM_CLASSCREATE, NULL, NULL);

	//window->AddCallProc(
	//	[this](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
	//{
	//	switch (uMsg)
	//	{
	//	case WM_CLASSCREATE:
	//	{
	//		InternalInitialize(hWnd);
	//		break;
	//	}
	//	case WM_INPUT:
	//	{
	//		InternalUpdateRawInput((HRAWINPUT)lParam);
	//		break;
	//	}
	//	default:
	//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	//	}
	//	return 0;
	//});
	//PostMessage(window->Get(), WM_CLASSCREATE, NULL, NULL);

	POINT ps;
	GetCursorPos(&ps);
	ScreenToClient(window->Get(),&ps);
	mousepos_ = { ps.x, ps.y };
}


void key::InternalInitialize(win32::window_handle window)
{
	// Raw Inputデバイスを設定する
	RAWINPUTDEVICE Rid[2];
	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x06;     //0x02にするとマウス。
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = window;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x02;     //0x02にするとマウス。
	Rid[1].dwFlags = 0;
	Rid[1].hwndTarget = window;
	RegisterRawInputDevices(Rid, 2, sizeof(Rid[0]));

}

void key::InternalUpdateRawInput(HRAWINPUT input_handle)
{
	UINT dwSize;
	win32::result result = E_FAIL;

	GetRawInputData((HRAWINPUT)input_handle, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb == NULL)
	{
		return;
	}

	if (GetRawInputData((HRAWINPUT)input_handle, RID_INPUT, lpb, &dwSize,
		sizeof(RAWINPUTHEADER)) != dwSize)
		OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEKEYBOARD && raw->data.keyboard.VKey < 255)
	{
		auto format = boost::tformat(TEXT(" Kbd: make=%04x Flags:%04x Reserved:%04x ExtraInformation:%08x, msg=%04x VK=%04x \n"))
			% raw->data.keyboard.MakeCode
			% raw->data.keyboard.Flags
			% raw->data.keyboard.Reserved
			% raw->data.keyboard.ExtraInformation
			% raw->data.keyboard.Message
			% raw->data.keyboard.VKey;
		OutputDebugString(format.str().c_str());
		if (raw->data.keyboard.Message == WM_KEYDOWN ||
			raw->data.keyboard.Message == WM_SYSKEYDOWN)
		{
			if (trigger_next_[static_cast<unsigned char>(raw->data.keyboard.VKey)] == false &&
				press_[static_cast<unsigned char>(raw->data.keyboard.VKey)] == false)
			{
				trigger_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = true;
				trigger_next_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = true;
			}
			else
			{
				trigger_next_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = false;
				press_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = true;
			}
		}
		if (raw->data.keyboard.Message == WM_KEYUP ||
			raw->data.keyboard.Message == WM_SYSKEYUP)
		{
			trigger_next_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = false;
			press_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = false;
			release_[static_cast<unsigned char>(raw->data.keyboard.VKey)] = true;
		}


	} else if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
		//	raw->data.mouse.usFlags,
		//	raw->data.mouse.ulButtons,
		//	raw->data.mouse.usButtonFlags,
		//	raw->data.mouse.usButtonData,
		//	raw->data.mouse.ulRawButtons,
		//	raw->data.mouse.lLastX,
		//	raw->data.mouse.lLastY,
		//	raw->data.mouse.ulExtraInformation);

		//OutputDebugString(szTempOutput);
		if (raw->data.mouse.usButtonFlags != 0)
		{
			//auto format = boost::format(TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"))
			//	% raw->data.mouse.usFlags
			//	% raw->data.mouse.ulButtons
			//	% raw->data.mouse.usButtonFlags
			//	% raw->data.mouse.usButtonData
			//	% raw->data.mouse.ulRawButtons
			//	% raw->data.mouse.lLastX
			//	% raw->data.mouse.lLastY
			//	% raw->data.mouse.ulExtraInformation;
			//OutputDebugString(format.str().c_str());
			auto detect = [this, &raw](ULONG msg,int btn)
			{
				if (raw->data.mouse.ulButtons == msg)
				{
					if (mousetrigger_[btn] == false &&
						mousepress_[btn] == false)
					{
						mousetrigger_[btn] = true;
					}
				}
				if (raw->data.mouse.ulButtons == (msg << 1))
				{
					mousepress_[btn] = false;
					mouserelease_[btn] = true;
				}
			};
			detect(RI_MOUSE_BUTTON_1_DOWN,0);
			detect(RI_MOUSE_BUTTON_2_DOWN,1);
			detect(RI_MOUSE_BUTTON_3_DOWN,2);
			detect(RI_MOUSE_BUTTON_4_DOWN,3);
			detect(RI_MOUSE_BUTTON_5_DOWN,4);
		}
		mousepos_.x += deltapos_.x = raw->data.mouse.lLastX;
		mousepos_.y += deltapos_.y = raw->data.mouse.lLastY;
		mousewheel_ = static_cast<short>(raw->data.mouse.usButtonData);
	}

	delete[] lpb;
}


void key::UpdateBuffer()
{
	release_.fill(false);
	for (auto i : stx::range(5))
	{
		if (mousetrigger_[i] && !mouserelease_[i])
			mousepress_[i] = true;
		mousetrigger_[i] = false;
	}
	mouserelease_.fill(false);
	trigger_.fill(false);
	deltapos_ = { 0, 0 };
	mousewheel_ = 0;
}