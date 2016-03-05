#pragma once
#include "Window.h"
#include "key.hpp"
#include "../unicode.hpp"

namespace win32
{

HINSTANCE GlobalArguments::hInstance;
LPSTR     GlobalArguments::lpCmdLine;
INT       GlobalArguments::nCmdShow;

void GlobalArguments::SetWinMainParams(HINSTANCE hInstance_, LPSTR lpCmdLine_, INT nCmdShow_)
{
	hInstance = hInstance_;
	lpCmdLine = lpCmdLine_;
	nCmdShow = nCmdShow_;
}

BaseWindow::BaseWindow(tstring classname)
{
	windowstatus_ = SW_HIDE;
	classname_ = classname;
}

BaseWindow::~BaseWindow()
{
}


//===== ポインタの設定 =====//
void BaseWindow::SetPointer(HWND hWnd)
{
	SetWindowLong(hWnd, GWL_USERDATA, (LONG)this);
	myhandle_ = hWnd;
}

//void BaseWindow::AddCallProc(FunctionProcedure func)
//{
//	mainprocs_.push_back(func);
//}
BaseWindow::this_type* BaseWindow::AddMessageProcMap(MessageProcPair proc)
{
	msg_proc_map_.insert(proc);
	return this;
}

HWND BaseWindow::Get()
{
	return myhandle_;
}

HINSTANCE BaseWindow::GethInstance()
{
	return GetModuleHandle(NULL);
}

tstring BaseWindow::GetName()
{
	std::array<char,256> buf;
	GetClassName(this->myhandle_, buf.data(), buf.size());
	return tstring(buf.data());
}

tstring BaseWindow::GetTitle()
{
	std::array<char, 256> buf;
	GetWindowText(this->myhandle_, buf.data(), buf.size());
	return tstring(buf.data());
}

tstring BaseWindow::GetTitleUtf8()
{
	return unicode::sjis2utf8(GetTitle());
}

float2 BaseWindow::GetWindowSize()
{
	RECT rc;
	::GetWindowRect(this->myhandle_, &rc);
	return{ static_cast<float>(rc.right - rc.left), static_cast<float>(rc.bottom - rc.top) };
}

float2 BaseWindow::GetClientSize()
{
	RECT rc;
	::GetClientRect(this->myhandle_, &rc);
	clientsize_ = { static_cast<float>(rc.right - rc.left), static_cast<float>(rc.bottom - rc.top) };
	return clientsize_;
}

int4 BaseWindow::GetClientRect()
{
	RECT rc;
	::GetClientRect(this->myhandle_, &rc);
	int4 ret = { rc.left,rc.top,rc.right,rc.bottom };
	return ret;
}

void BaseWindow::SetClientSize(float2 size)
{
	RECT rc;
	rc.right = size.x;
	rc.bottom = size.y;
	AdjustWindowRectEx(&rc, GetWindowLong(this->myhandle_, GWL_STYLE), false, GetWindowLong(this->myhandle_, GWL_EXSTYLE));
	int32_t cx = rc.right - rc.left;
	int32_t cy = rc.bottom - rc.top;
	SetWindowPos(this->myhandle_, NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_ASYNCWINDOWPOS);
}

void BaseWindow::Initialize()
{
	if (!key_)key_ = std::make_shared<key>();
	key_->Initialize(this->shared_from_this());
	LoadPlacement();
	this->Show();
}

void BaseWindow::Destroy()
{
	UnregisterClass(classname_.c_str(), GlobalArguments::hInstance);
}

key_ptr BaseWindow::GetKey()
{
	return key_;
}

generic_handle BaseWindow::AddControl(tstring classname, tstring controlname, DWORD controlstyle, boost::filesystem::path library, MessageProc initialize_callback)
{
	static uint32_t handle_counter = 0;
	generic_handle handle = (generic_handle)++handle_counter;

	AddMessageProcMap({ WM_CLASSCREATE,[=](HWND hwnd, WPARAM wp, LPARAM lp)->LRESULT {

		if (this->control_map_.find(handle) != this->control_map_.end())return 0;

		BaseControl control;

		control.Load(library.string());

		control.handle = CreateWindowEx(
			0,
			classname.c_str(),
			controlname.c_str(),
			controlstyle,
			0,
			0,
			0,
			0,
			hwnd,
			(HMENU)handle,
			this->GethInstance(),
			this);

		control.offset = { 0,0,0,0 };

		control.size_handler = nullptr;

		this->control_map_.insert({ handle, control });

		if (initialize_callback)
		{
			initialize_callback(control.handle, wp, lp);
		}

		return 0;
	} });


	AddMessageProcMap({ WM_SIZE,[&](HWND hwnd, WPARAM wp, LPARAM lp)->LRESULT {
		auto &control = this->control_map_[handle];
		if(control.size_handler)
			return control.size_handler(control.handle, wp, lp);
		else
			return MoveWindow(control.handle, control.offset.x, control.offset.y, LOWORD(lp) - control.offset.z, HIWORD(lp) - control.offset.w, TRUE);
	} });

	AddMessageProcMap({WM_DESTROY,[=](HWND, WPARAM, LPARAM)->LRESULT {
		DestroyWindow(this->control_map_[handle].handle);
		return 0;
	} });

	return handle;
}

void BaseWindow::FlushControl()
{
	SendMessage(this->Get(), WM_CLASSCREATE, 0, 0);
}

void BaseWindow::MoveControl(generic_handle handle, int4 offset)
{
	this->control_map_[handle].offset = offset;
}

void BaseWindow::ChangeMoveMethod(generic_handle handle, MessageProc proc)
{
	this->control_map_[handle].size_handler = proc;
}

void BaseWindow::SetCommandCallback(generic_handle handle, MessageProc proc)
{
	AddMessageProcMap({ WM_COMMAND,[=](HWND hwnd, WPARAM wp, LPARAM lp)->LRESULT {
		auto &control = this->control_map_[handle];
		if (LOWORD(wp) != (WORD)handle)return 0;
		return proc(control.handle, wp, lp);
	}
	});
}

void BaseWindow::SetParent(BaseWindowPtr parent)
{
	parent_ = parent;
}


//===== ウィンドウプロシージャの呼び出し =====//
LRESULT CALLBACK BaseWindow::CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//＿プロパティリストからthisポインタを取得
	BaseWindow* thisPtr = (BaseWindow*)GetWindowLong(hWnd, GWL_USERDATA);

	//＿thisポインタが取得できなかった場合...
	if (!thisPtr)
	{
		//＿ウィンドウの作成時の場合...
		if (message == WM_CREATE)
		{
			thisPtr = (BaseWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		}

		//＿thisポインタが取得できた場合...
		if (thisPtr)
		{
			//＿プロパティリストにオブジェクトハンドル(thisポインタ)を設定する
			thisPtr->SetPointer(hWnd);
		}
	}

	//＿thisポインタが取得できた場合...
	if (thisPtr && !thisPtr->msg_proc_map_.empty())
	{
		if (thisPtr->msg_proc_map_.count(message) > 0)
		{
			auto range = thisPtr->msg_proc_map_.equal_range(message);
			LRESULT ret = 0;

			for (auto it = range.first; it != range.second; it++)
			{
				auto &item = *it;
				ret = item.second(hWnd, wParam, lParam);
			}
			return ret;
		}
	}
	//if (thisPtr && !thisPtr->mainprocs_.empty())
	//{
	//	LRESULT lResult;
	//	for (auto proc : thisPtr->mainprocs_)
	//	{
	//		lResult = proc(hWnd, message, wParam, lParam);
	//	}

	//	return lResult;
	//}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void BaseWindow::Show()
{
	if (windowstatus_ != SW_SHOW)
		ShowWindow(myhandle_, (windowstatus_ = SW_SHOW));
	else
	{
		ShowWindow(myhandle_, (windowstatus_ = GlobalArguments::nCmdShow));
	}
	Update();
}

void BaseWindow::ShowFront()
{
	SetWindowPos(myhandle_, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
}

void BaseWindow::Update()
{
	UpdateWindow(myhandle_);
}

void BaseWindow::ProcessHolder()
{

}

void BaseWindow::ProcessHolderWrapper()
{
	BaseWindow::ProcessHolderBeforeWrapper();
	this->ProcessHolder();
	BaseWindow::ProcessHolderAfterWrapper();
}

void BaseWindow::ProcessHolderBeforeWrapper()
{
}

void BaseWindow::ProcessHolderAfterWrapper()
{
}


unsigned int BaseWindow::ProcessMessage()
{
	//while(PeekMessage(&msg_, NULL, 0, 0 , PM_NOREMOVE))
	//{
	//		if (!GetMessage(&msg_, NULL, 0, 0)) /* メッセージ処理 */
	//			return msg_.wParam;
	//		TranslateMessage(&msg_);
	//		DispatchMessage(&msg_);
	//}
	while(PeekMessage(&msg_, NULL, 0, 0 , PM_REMOVE))
	{
			TranslateMessage(&msg_);
			DispatchMessage(&msg_);
	}
	if (msg_.message == WM_QUIT)
		return 2;
	else
		return 0;
}


unsigned int BaseWindow::SimplePollingMessage()
{
	TranslateMessage(&msg_);
	DispatchMessage(&msg_);
	return 1;
}

void BaseWindow::Post(UINT message, WPARAM wParam, LPARAM lParam)
{
	PostMessage(Get(), message, wParam, lParam);
}

bool BaseWindow::isLive()
{
	return live;
}

void BaseWindow::SetDead()
{
	live = false;
}

BaseWindowPtr BaseWindow::GetParent()
{
	return parent_;
}

void BaseWindow::SavePlacement(int2 pos)
{
	WINDOWPLACEMENT placement;
	placement.length = sizeof(placement);

	GetWindowPlacement(this->Get(), &placement);

	int4 place = { pos.x,pos.y,
		(int)(placement.rcNormalPosition.right - placement.rcNormalPosition.left),
		(int)(placement.rcNormalPosition.bottom - placement.rcNormalPosition.top) };

	bool maximized = false;

	if (IsZoomed(this->Get()))
		maximized = true;
	if (IsIconic(this->Get()))
		return;

	boost::property_tree::path config_path = "app";
	config_path /= this->classname_;
	auto place_path = (config_path / "place").dump();
	auto maximized_path = (config_path / "maximized").dump();

	stx::config_file::GetRefInstance()[place_path] << place;
	stx::config_file::GetRefInstance()[maximized_path] << maximized;
	stx::config_file::GetInstance()->save();
}

void BaseWindow::LoadPlacement()
{
	boost::property_tree::path config_path = "app";
	config_path /= this->classname_;
	auto place_path = (config_path / "place").dump();
	auto maximized_path = (config_path / "maximized").dump();

	int4 place = stx::config_file::GetRefInstance()[place_path].get_default<int4>({ 0,0,-1,-1 });
	bool maximized = stx::config_file::GetRefInstance()[maximized_path].get_default<bool>(false);

	if (place.x == 0 && place.y == 0)
	{
		// 初期設定の時は画面の真ん中になるようにする
		HMONITOR moni;
		moni = MonitorFromPoint({0, 0}, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEX info;
		info.cbSize = sizeof(MONITORINFOEX);
		GetMonitorInfo(moni, &info);
		place.x = (info.rcWork.right - place.z) * 0.5f;
		place.y = (info.rcWork.bottom - place.w) * 0.5f;
	}

	if (place.r[2] > 0 && place.r[3] > 0)
		SetWindowPos(this->myhandle_, NULL, place.x, place.y, place.z, place.w, SWP_NOZORDER);
	if (maximized)
		ShowWindow(this->myhandle_, SW_MAXIMIZE);

	AddMessageProcMap({ WM_MOVE,[&](HWND hwnd, WPARAM wp, LPARAM lp)->LRESULT {
		SavePlacement({LOWORD(lp),HIWORD(lp)});
		return 0;
	} });

}


void BaseControl::Load(tstring library_path)
{
	library_handle = LoadLibrary(library_path.c_str());
}

void BaseControl::Release()
{
	FreeLibrary(library_handle);
}

};