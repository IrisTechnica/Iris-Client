#pragma once
#include <stx/Property.hpp>
#include <stx/singleton.hpp>
#include "detail\directx.h"

class Graphics : public stx::singleton<Graphics>
{
#pragma region Internal Data Members
private:
	dx11::device device;
	dx11::devicecontext deviceContext;
	dx11::dxgiswapchain	swapChain;
	dx11::display_format displayFormat;
	dx11::feature_level	featureLevel;

	bool isFullScreen;
#pragma endregion

#pragma region Public Properties
public:
	ReadOnlyProperty<dx11::device> Device = device;
	ReadOnlyProperty<dx11::devicecontext> DeviceContext = deviceContext;
	ReadOnlyProperty<dx11::dxgiswapchain> SwapChain = swapChain;
	ReadOnlyProperty<dx11::display_format> DisplayFormat = displayFormat;
	ReadOnlyProperty<dx11::feature_level> FeatureLevel = featureLevel;
#pragma endregion

#pragma region Public Methods
public:
	Graphics();
	virtual ~Graphics();

	void Initialize(win32::BaseWindowPtr window, bool isFullScreen = false);

#pragma endregion

#pragma region Private Methods
private:
	void InitDisplayMode(const dx11::display_format& format);

#pragma endregion
};

