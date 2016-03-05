#include "window.h"
#pragma once

template<class _WindowPtr>
inline _WindowPtr FactoryWindow::CreateNormalWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent)
{
	return CreateCustomWindow<_WindowPtr>(classname, windowname, rect, CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW, WS_OVERLAPPEDWINDOW | WS_VISIBLE, parent);
}

template<class _WindowPtr>
inline _WindowPtr FactoryWindow::CreateInvisibleWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent)
{
	return CreateCustomWindow<_WindowPtr>(classname, windowname, rect, NULL, (NULL | WS_EX_TOOLWINDOW) & ~WS_VISIBLE, parent);
}

template<class _WindowPtr>
inline _WindowPtr FactoryWindow::CreateCustomWindow(tstring classname, tstring windowname, int4 rect, UINT classstyle, DWORD windowstyle, ParentWindowPtr parent)
{
	_WindowPtr bwnd;

	auto check = (*WindowManager::GetInstance())[classname];
	if (check)
	{
		check->ShowFront();
		return bwnd;
	}

	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		classstyle,
		BaseWindow::CallProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		classname.c_str(),
		NULL
	};

	// ウィンドウクラスの登録
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("ウィンドウクラス構造体の初期化エラー"), _T(""), MB_OK);
	}

	bwnd.reset(new _WindowPtr::element_type(classname));

	win32::window_handle parent_handle = NULL;
	if (parent)
	{
		parent_handle = parent->Get();
		bwnd->SetParent(parent);
	}

	HWND hWnd = CreateWindowEx(
		0,
		classname.c_str(),
		windowname.c_str(),
		windowstyle,
		rect.x,
		rect.y,
		rect.z,
		rect.w,
		0,//parent_handle,
		NULL,
		hInstance,
		reinterpret_cast<LPVOID>(bwnd.get()));
	bwnd->SetPointer(hWnd);
	bwnd->AddMessageProcMap({ WM_DESTROY,[bwnd](HWND,WPARAM,LPARAM)->LRESULT {
		PostQuitMessage(0);
		bwnd->SetDead();
		return 0;
	} });
	//bwnd->AddCallProc(
	//	[](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)->LRESULT
	//{
	//	switch (uMsg)
	//	{
	//	case WM_DESTROY:
	//		PostQuitMessage(0);
	//		break;
	//	default:
	//		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	//	}
	//	return 0;
	//});

	WindowManager::GetInstance()->Regist(classname, bwnd);

	bwnd->Initialize();

	return bwnd;
}

template<class _WindowPtr>
inline _WindowPtr win32::FactoryWindow::CreatePopupWindow(tstring classname, tstring windowname, int4 rect, ParentWindowPtr parent)
{
	return CreateCustomWindow<_WindowPtr>(classname, windowname, rect, CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW, WS_POPUP | WS_VISIBLE, parent);
}

