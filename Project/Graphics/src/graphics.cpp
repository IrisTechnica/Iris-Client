#include "graphics.h"

#include "stx/logger.hpp"


Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::Initialize(win32::BaseWindowPtr window, bool isFullScreen)
{
	stx::logger::scope_message<stx::logger::begin_end> msg(_T("Init Device"));

	win32::result result = E_FAIL;

	this->InitDisplayMode(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto matchdesc = this->FindFitDesc(window->GetClientSize());

	this->CreateDevice();
}
