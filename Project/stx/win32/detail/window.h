#pragma once
#include <Windows.h>

#include <functional>
#include <vector>
#include <unordered_map>

#include "../../tstring_symbol.hpp"
#include "../../enumerable.hpp"

namespace win32
{

typedef HRESULT		result;
typedef HWND		window_handle;
typedef HINSTANCE	instance_handle;
typedef MSG			window_message;
typedef HDC			device_context;
typedef HPEN		pen;
typedef HBRUSH		brush;
typedef PAINTSTRUCT	paint_struct;

typedef HANDLE		generic_handle;

typedef unsigned short      word; // 2byte
typedef unsigned long      dword; // 4byte
typedef unsigned long long qword; // 8byte

};

#ifdef	WM_CLASSCREATE
#undef	WM_CLASSCREATE
#endif//WM_CLASSCREATE
#define WM_CLASSCREATE (WM_GETMINMAXINFO + 0x0001)

#ifdef	WM_KEYUPDATE
#undef	WM_KEYUPDATE
#endif//WM_KEYUPDATE
#define WM_KEYUPDATE (WM_GETMINMAXINFO + 0x0002)
