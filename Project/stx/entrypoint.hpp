#pragma once
#include "config.hpp"

#ifdef _WIN32 // Windows Enviroment
#include <Windows.h>
#define WIN32_UNREFERENCED_PARAMETER(param) UNREFERENCED_PARAMETER((param))
#if defined(DEBUGCONSOLE_SHOW)
#pragma comment(linker,"/SUBSYSTEM:CONSOLE")
#define ENTRYPOINT \
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow); \
int main(int argc,char *argv[]) \
{ \
return _tWinMain(GetModuleHandle(NULL), (HINSTANCE)argc, (LPTSTR)argv, SW_SHOW); \
}\
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#else
#pragma comment(linker,"/SUBSYSTEM:WINDOWS")
#define ENTRYPOINT int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif

#else // Other entry type Enviroments
#define WIN32_UNREFERENCED_PARAMETER(param)
#define ENTRYPOINT int main()
#endif


#if defined(DEBUG) || defined(_DEBUG)
#define	MEMCHECK	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);
#else
#define MEMCHECK
#endif
