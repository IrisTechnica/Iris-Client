#pragma once
#include <stx/Property.hpp>
#include "detail\directx.h"

// ï`âÊÉGÉäÉAÇÃä«óù
class Canvas
{
#pragma region Internal Temporary Class

	class ScopedCanvas
	{
#pragma region Public Methods
	public:
		explicit ScopedCanvas(Canvas *const right);
		ScopedCanvas() = delete;
		~ScopedCanvas();

#pragma endregion
	private:
		Canvas *const entity;
	};
	typedef std::shared_ptr<ScopedCanvas> ScopedCanvasPtr;


#pragma endregion
#pragma region Internal Data Members
private:
	dx11::rendertargetview renderTargetView;
	dx11::depthstencilview depthStencilView;
	std::array<dx11::viewport , 1> viewPort;

	float2 renderArea;

#pragma endregion

#pragma region Public Properties
public:
	ReadOnlyProperty<dx11::rendertargetview> RenderTargetView = renderTargetView;
	ReadOnlyProperty<dx11::depthstencilview> DepthStencilView = depthStencilView;
	ReadOnlyProperty<std::array<dx11::viewport, 1>> ViewPort = viewPort;
	ReadOnlyProperty<float2> RenderArea = renderArea;

#pragma endregion

#pragma region Public Methods
public:
	Canvas();
	~Canvas();

	void Initialize(float2 renderArea);
	ScopedCanvasPtr Apply(); // Auto bind and unbind view

#pragma endregion

#pragma region Private Methods
private:
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateViewPort();

#pragma endregion

};
