#include "canvas.h"
#include <stx/logger.hpp>
#include "graphics.h"


Canvas::Canvas()
{
}


Canvas::~Canvas()
{
}

void Canvas::Initialize(float2 renderArea)
{
	this->renderArea = renderArea;
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateViewPort();
}

Canvas::ScopedCanvasPtr Canvas::Apply()
{
	return std::make_shared<ScopedCanvas>(this);
}

void Canvas::CreateRenderTargetView()
{
	stx::logger::scope_message<stx::logger::begin_end> msg("Create RenderTargetView");

	win32::result result = E_FAIL;

	dx11::texture2d backbuffer;

	auto graphics = Graphics::GetInstance();

	//Get backbuffer from swapchain	
	result = graphics->SwapChain->GetBuffer(0, __uuidof(dx11::itexture2d), (void**)backbuffer.ToCreator());
	if (FAILED(result))
	{
		return stx::logger::Error("failed get backbuffer");
		return;
	}
	stx::logger::Info("succeeded get backbuffer");

	//Create render target view
	result = graphics->Device->CreateRenderTargetView(backbuffer.Get(), nullptr, this->renderTargetView.ToCreator());
	if (FAILED(result))
	{
		stx::logger::Error("failed create render target view");
		return;
	}
	stx::logger::Info("succeeded create render target view");

	return;

}

void Canvas::CreateDepthStencilView()
{
	win32::result result = E_FAIL;

	dx11::texture2d			depthbuffer;
	dx11::texture2d_desc	depthbuffer_desc;

	dx11::depthstencilview_desc	depthstencilview_desc;

	stx::logger::scope_message<stx::logger::begin_end> msg("Create DepthStencilView");

	auto graphics = Graphics::GetInstance();
	
	auto s = graphics->SwapChain;
	if (graphics->SwapChain == nullptr)
	{
		stx::logger::Error("swap chain is empty");
		return;
	}

	dx11::swapchain_desc swapchain_desc;
	result = graphics->SwapChain->GetDesc(&swapchain_desc);
	if (FAILED(result))
	{
		stx::logger::Error("failed get swapchain desc");
		return;
	}
	stx::logger::Info("succeeded get swapchain desc");

	ZeroMemory(&depthbuffer_desc, sizeof(depthbuffer_desc));
	depthbuffer_desc.Width = swapchain_desc.BufferDesc.Width;
	depthbuffer_desc.Height = swapchain_desc.BufferDesc.Height;
	depthbuffer_desc.MipLevels = 1;
	depthbuffer_desc.ArraySize = 1;
	depthbuffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthbuffer_desc.SampleDesc.Count = swapchain_desc.SampleDesc.Count;
	depthbuffer_desc.SampleDesc.Quality = swapchain_desc.SampleDesc.Quality;
	depthbuffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depthbuffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthbuffer_desc.CPUAccessFlags = NULL;
	depthbuffer_desc.MiscFlags = NULL;

	//create texture for depthstencil buffer
	result = graphics->Device->CreateTexture2D(&depthbuffer_desc, nullptr, depthbuffer.ToCreator());
	if (FAILED(result))
	{
		stx::logger::Error("failed create depthbuffer");
		return;
	}
	stx::logger::Info("succeeded create depthbuffer");

	ZeroMemory(&depthstencilview_desc, sizeof(depthstencilview_desc));

	switch (depthbuffer_desc.Format)
	{
		//cannot use 8bit option
		//case DXGI_FORMAT_R8_TYPELESS:
		//	depthstencilview_desc.Format = DXGI_FORMAT_R8_UNORM;
		//	break;
	case DXGI_FORMAT_R16_TYPELESS:
		depthstencilview_desc.Format = DXGI_FORMAT_D16_UNORM;
		break;
	case DXGI_FORMAT_R32_TYPELESS:
		depthstencilview_desc.Format = DXGI_FORMAT_D32_FLOAT;
		break;
	case DXGI_FORMAT_R24G8_TYPELESS:
		depthstencilview_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		break;
	default:
		depthstencilview_desc.Format = depthbuffer_desc.Format;
	}

	//if used multisample
	if (swapchain_desc.SampleDesc.Count > 1)
		depthstencilview_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	else
		depthstencilview_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	depthstencilview_desc.Texture2D.MipSlice = 0;
	//Create depthstencilview
	result = graphics->Device->CreateDepthStencilView(depthbuffer.Get(), &depthstencilview_desc, this->depthStencilView.ToCreator());
	if (FAILED(result))
	{
		stx::logger::Error("failed create depthstencilview");
		return;
	}
	stx::logger::Info("succeeded create depthstencilview");

	return;
}

void Canvas::CreateViewPort()
{
	stx::logger::scope_message<stx::logger::begin_end> msg(_T("Create ViewPort"));

	std::array<dx11::viewport, 1> viewport;
	viewport[0].TopLeftX = 0;
	viewport[0].TopLeftY = 0;
	viewport[0].Width = renderArea.x;
	viewport[0].Height = renderArea.y;
	viewport[0].MinDepth = 0.0f;
	viewport[0].MaxDepth = 1.0f;

	this->viewPort = viewport;
}

Canvas::ScopedCanvas::ScopedCanvas(Canvas * const right)
	: entity(right)
{
	auto graphics = Graphics::GetInstance();

	graphics->DeviceContext->RSSetViewports(this->entity->ViewPort().size(), this->entity->ViewPort().data());
}

Canvas::ScopedCanvas::~ScopedCanvas()
{
}
