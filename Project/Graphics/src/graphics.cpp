#include "graphics.h"
#include "commongraphics.hpp"

#include "../logger.hpp"
#include "../range.hpp"

#include "../stx/win32/windowmanager.h"
#include "graphicwindow.h"

namespace dx11
{

	Graphics::Graphics()
	{
		this->isinitdisplay_mode_ = false;
		this->isusedepthbuffer_ = true;
		this->isfullscreen_ = false;
	}


	Graphics::~Graphics()
	{
	}

	win32::result Graphics::InitDevice(win32::BaseWindowPtr _window, bool _isfullscreen, GraphicsPtr reference)
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Init App");

		win32::result result = E_FAIL;

		this->graphic_ = stx::make_shared<dx11::igraphics_essential>();

		this->window_ = _window->Get();
		if (!this->isinitdisplay_mode_)
			this->InitDisplayMode(DXGI_FORMAT_R8G8B8A8_UNORM); //DXGI_FORMAT_R16G16B16A16_FLOAT

		auto matchdesc = this->FindFitDesc(_window->GetClientSize());

		if(!reference)
			this->CreateDevice();
		else
		{
			this->graphic_->device = reference->graphic_->device;
			this->graphic_->devicecontext = reference->graphic_->devicecontext;
		}
		this->CreateSwapChain(matchdesc, true);
		this->CreateRenderTargetView();
		this->CreateDepthStencilView();

		this->graphic_->devicecontext->OMSetRenderTargets(
			1,
			&this->graphic_->rendertargetview,
			this->graphic_->depthstencilview.Get());

		this->CreateViewPort();
		this->SetDefaultRasterizeMethod();
		this->SetDefaultDepthStencilState();

		//fullscreen check
		this->isfullscreen_ = _isfullscreen;
		if (this->isfullscreen_)
		{
			result = this->ChangeWindowMode();
			if (FAILED(result))
				return E_FAIL;
		}

		return S_OK;

	}

	win32::result Graphics::InitDisplayMode(const dx11::display_format format)
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Init DisplayMode");

		win32::result result = E_FAIL;
		dx11::displaymode_desc * desc = nullptr;
		stx::com_ptr<IDXGIOutput> output;

		using_format_ = format;

		{
			stx::logger::Return(2);
			stx::logger::scope_message<stx::logger::start_complete> msg("Enumerate of Display Mode");


			//アダプタの作成
			result = this->CreateAdapter();
			if (FAILED(result))
			{
				stx::logger::Error("failed create adapter.");
				return E_FAIL;
			}
			stx::logger::Info("succeeded create adapter.");


			//プライマリ取得
			result = this->graphic_->adapter->EnumOutputs(0, &output);
			if (FAILED(result))
			{
				stx::logger::Error("failed get primary display.");
				return E_FAIL;
			}
			stx::logger::Info("succeeded get primary display.");

			unsigned int _displaymode_num;
			//リスト数取得
			result = output->GetDisplayModeList(format, 0, &_displaymode_num, nullptr);
			if (FAILED(result))
			{
				stx::logger::Error("failed enumrate of display mode list.");
				return E_FAIL;
			}
			stx::logger::Info("succeeded enumrate of display mode list.");

			desc = new dx11::displaymode_desc[_displaymode_num];

			//ディスプレイモードの一覧取得
			result = output->GetDisplayModeList(format, 0, &_displaymode_num, desc);
			if (FAILED(result))
			{
				stx::logger::Error("failed get display mode list.");
				if (desc)delete[] desc;
				return E_FAIL;
			}
			stx::logger::Info("succeeded get display mode list.");
			stx::logger::Return(1);

			for (unsigned i : stx::range(_displaymode_num - 1))
			{
				this->displaymodesdesclist_.push_back(desc[i]);
				stx::logger::Debug("Resolution [ %d : %d  @Rate[ %d / %d ] ]",
					desc[i].Width,
					desc[i].Height,
					desc[i].RefreshRate.Denominator,
					desc[i].RefreshRate.Numerator);
			}

			if (desc)delete[] desc;
			this->isinitdisplay_mode_ = true;
			result = S_OK;
		}
		return result;
	}

	dx11::devicecontext Graphics::GetDeferredContext(unsigned int worker_thread)
	{
		if (this->deferred_context_map_.find(worker_thread)
			!= this->deferred_context_map_.end())
			return this->deferred_context_map_[worker_thread];

		dx11::devicecontext deferredcontext;
		this->graphic_->device->CreateDeferredContext(NULL, &deferredcontext);
		this->deferred_context_map_.insert({ worker_thread,deferredcontext });
		return deferredcontext;
	}

	void Graphics::ResizeSwapChain()
	{
		this->graphic_->devicecontext->OMSetRenderTargets(0, 0, 0);
		this->graphic_->rendertargetview = nullptr;
		this->graphic_->depthstencilview = nullptr;
		
		win32::result hr;
		hr = this->graphic_->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr))return stx::logger::Error("can't resizebuffer");
		CreateRenderTargetView();
		CreateDepthStencilView();
		this->graphic_->devicecontext->OMSetRenderTargets(1, &this->graphic_->rendertargetview, this->graphic_->depthstencilview.Get());
		CreateViewPort();
	}

	inline win32::result Graphics::CreateAdapter()
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Create Adapter");

		win32::result result = E_FAIL;
		dx11::dxgifactory factory;

		if (this->graphic_->adapter == nullptr)
		{
			result = CreateDXGIFactory(__uuidof(dx11::idxgifactory), (void**)(factory.ToCreator()));
			if (FAILED(result))
				return result;

			result = factory->EnumAdapters(0, &this->graphic_->adapter);
			if (FAILED(result))
				return result;
		}

		return S_OK;
	}

	inline win32::result Graphics::CreateDevice()
	{
		stx::logger::scope_message<stx::logger::start_complete> msg("Create Device");

		win32::result result = E_FAIL;

		std::array<dx11::feature_level, 7> featurelevel =
		{
			D3D_FEATURE_LEVEL_11_1,	 // Direct3D 11.1 SM 5
			D3D_FEATURE_LEVEL_11_0,  // Direct3D 11.0 SM 5
			D3D_FEATURE_LEVEL_10_1,  // Direct3D 10.1 SM 4
			D3D_FEATURE_LEVEL_10_0,  // Direct3D 10.0 SM 4
			D3D_FEATURE_LEVEL_9_3,   // Direct3D 9.3  SM 3
			D3D_FEATURE_LEVEL_9_2,   // Direct3D 9.2  SM 2
			D3D_FEATURE_LEVEL_9_1,   // Direct3D 9.1  SM 2
		};

		unsigned device_flag = 0;
#ifdef __debug
		device_flag = D3D11_CREATE_DEVICE_DEBUG;
		stx::logger::Debug("use debug layer.");
#endif

		//first try initialize on directx11
		result = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			device_flag,
			featurelevel.data(),
			1,						//11_0のみに絞る
			D3D11_SDK_VERSION,
			this->graphic_->device.ToCreator(),
			&this->feature_level_,	//実際に使用されたモード
			this->graphic_->devicecontext.ToCreator()
			);

		if (FAILED(result))
		{
			stx::logger::Warning("failed create directx11 mode device. retry on auto mode.");
			//secondary initialize on directx9 to 10
			result = D3D11CreateDevice(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				device_flag,
				featurelevel.data(),
				featurelevel.size(),	//自動モード
				D3D11_SDK_VERSION,
				this->graphic_->device.ToCreator(),
				&this->feature_level_,	//実際に使用されたモード
				this->graphic_->devicecontext.ToCreator()
				);
			if (FAILED(result))
			{
				stx::logger::Error("failed create device.");
			}
		}
		stx::logger::Info("succeeded create device.");

#ifdef __debug
		{
			stx::logger::Debug("check used adapter type.");
			dx11::adapter_desc desc;
			this->graphic_->adapter->GetDesc(&desc);
			tstring str = unicode::ToTString(desc.Description);

			switch (this->feature_level_)
			{
			case D3D_FEATURE_LEVEL_11_1:
				stx::logger::Debug("DirectX 11.1 SM(5) %s", str);
				break;
			case D3D_FEATURE_LEVEL_11_0:
				stx::logger::Debug("DirectX 11.0 SM(5) %s", str);
				break;
			case D3D_FEATURE_LEVEL_10_1:
				stx::logger::Debug("DirectX 10.1 SM(4) %s", str);
				break;
			case D3D_FEATURE_LEVEL_10_0:
				stx::logger::Debug("DirectX 10.0 SM(4) %s", str);
				break;
			case D3D_FEATURE_LEVEL_9_3:
				stx::logger::Debug("DirectX 9.3 SM(3) %s", str);
				break;
			case D3D_FEATURE_LEVEL_9_2:
				stx::logger::Debug("DirectX 9.2 SM(2) %s", str);
				break;
			case D3D_FEATURE_LEVEL_9_1:
				stx::logger::Debug("DirectX 9.1 SM(1) %s", str);
				break;
			}
		}

#endif

		return S_OK;
	}

	inline win32::result Graphics::CreateSwapChain(const dx11::displaymode_desc* desc, bool enablemultisample)
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Create SwapChain");

		win32::result result = E_FAIL;

		dx11::swapchain_desc swc_desc;
		ZeroMemory(&swc_desc, sizeof(swc_desc));

		memcpy(&(swc_desc.BufferDesc), desc, sizeof(dx11::displaymode_desc));

		swc_desc.BufferDesc.Format = this->using_format_;
		swc_desc.BufferDesc.Width = 0;
		swc_desc.BufferDesc.Height = 0;
		swc_desc.BufferDesc.RefreshRate.Numerator = 60;
		swc_desc.BufferDesc.RefreshRate.Denominator = 1;
		swc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swc_desc.BufferCount = 2;
		swc_desc.OutputWindow = this->window_;
		swc_desc.Windowed = TRUE;
		swc_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		dx11::dxgifactory factory;
		result = this->graphic_->adapter->GetParent(__uuidof(dx11::idxgifactory), (void**)factory.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed get factory from adapter");
			return E_FAIL;
		}
		stx::logger::Info("suceeded get factory from adapter");

		//defaut setting (none multisample)
		swc_desc.SampleDesc.Count = 1;
		swc_desc.SampleDesc.Quality = 0;

		if (enablemultisample)
		{
			unsigned quality;
			//enable multisample
			for (int i = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i >= 1; i >>= 1)
			{
				result = this->graphic_->device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, (unsigned)i, &quality);
				if (FAILED(result))
				{
					stx::logger::Error("failed check multisample quality levels.");
					return E_FAIL;
				}
				stx::logger::Info("succeeded check multisample quality levels.");

				swc_desc.SampleDesc.Count = i;
				if (quality != 0)
				{
					swc_desc.SampleDesc.Quality = quality - 1;
					result = factory->CreateSwapChain(this->graphic_->device.Get(), &swc_desc, this->graphic_->swapchain.ToCreator());
					if (SUCCEEDED(result))
					{
						stx::logger::Info("set Multisample => Count : Quality [%d : %d] ... OK", swc_desc.SampleDesc.Count, swc_desc.SampleDesc.Quality);
						break;
					}
				}
				stx::logger::Info("set Multisample => Count : Quality [%d : %d] ... NG", swc_desc.SampleDesc.Count, swc_desc.SampleDesc.Quality);
			}
			if (this->graphic_->swapchain == nullptr)
			{
				stx::logger::Error("failed create swapchain.");
				return E_FAIL;
			}
		} else
		{
			result = factory->CreateSwapChain(this->graphic_->device.Get(), &swc_desc, this->graphic_->swapchain.ToCreator());
			if (FAILED(result))
			{
				stx::logger::Error("failed create swapchain without multisample.");
				return E_FAIL;
			}
			stx::logger::Info("succeeded create swapchain without multisample.");
		}

		stx::logger::Debug("Selected Display Mode => Resolution [ %d : %d  @Rate[ %d / %d ] ]",
			desc->Width,
			desc->Height,
			desc->RefreshRate.Denominator,
			desc->RefreshRate.Numerator);


		return S_OK;

	}

	inline win32::result Graphics::CreateRenderTargetView()
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Create RenderTargetView");

		win32::result result = E_FAIL;

		dx11::texture2d backbuffer;

		//Get backbuffer from swapchain	
		result = this->graphic_->swapchain->GetBuffer(0, __uuidof(dx11::itexture2d), (void**)backbuffer.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed get backbuffer");
			return E_FAIL;
		}
		stx::logger::Info("succeeded get backbuffer");

		//Create render target view
		result = this->graphic_->device->CreateRenderTargetView(backbuffer.Get(), nullptr, this->graphic_->rendertargetview.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed create render target view");
			return E_FAIL;
		}
		stx::logger::Info("succeeded create render target view");

		return S_OK;

	}

	inline win32::result Graphics::CreateDepthStencilView()
	{

		win32::result result = E_FAIL;

		dx11::texture2d			depthbuffer;
		dx11::texture2d_desc	depthbuffer_desc;

		dx11::depthstencilview_desc	depthstencilview_desc;

		if (this->isusedepthbuffer_ == false)
		{
			stx::logger::Info("do not use depthstencilview");
			return S_FALSE;
		}

		stx::logger::scope_message<stx::logger::begin_end> msg("Create DepthStencilView");

		if (this->graphic_->swapchain == nullptr)
		{
			stx::logger::Error("swap chain is empty");
			return E_FAIL;
		}

		dx11::swapchain_desc swapchain_desc;
		result = this->graphic_->swapchain->GetDesc(&swapchain_desc);
		if (FAILED(result))
		{
			stx::logger::Error("failed get swapchain desc");
			return E_FAIL;
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
		result = this->graphic_->device->CreateTexture2D(&depthbuffer_desc, nullptr, depthbuffer.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed create depthbuffer");
			return E_FAIL;
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
		result = this->graphic_->device->CreateDepthStencilView(depthbuffer.Get(), &depthstencilview_desc, this->graphic_->depthstencilview.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed create depthstencilview");
			return E_FAIL;
		}
		stx::logger::Info("succeeded create depthstencilview");

		return S_OK;

	}

	inline win32::result Graphics::CreateViewPort()
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Create ViewPort");

		win32::result result = E_FAIL;

		if (this->graphic_->swapchain == nullptr)
		{
			stx::logger::Error("swap chain is empty");
			return E_FAIL;
		}

		dx11::swapchain_desc swapchain_desc;
		result = this->graphic_->swapchain->GetDesc(&swapchain_desc);
		if (FAILED(result))
		{
			stx::logger::Error("failed get swapchain desc");
			return E_FAIL;
		}
		stx::logger::Info("succeeded get swapchain desc");

		std::array<dx11::viewport, 1> viewport;
		viewport[0].TopLeftX = 0;
		viewport[0].TopLeftY = 0;
		viewport[0].Width = static_cast<float>(swapchain_desc.BufferDesc.Width);
		viewport[0].Height = static_cast<float>(swapchain_desc.BufferDesc.Height);
		viewport[0].MinDepth = 0.0f;
		viewport[0].MaxDepth = 1.0f;

		this->graphic_->devicecontext->RSSetViewports(1, viewport.data());

		return S_OK;
	}

	inline win32::result Graphics::SetMakeWindowAssociation()
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Change WindowMode");

		win32::result result = E_FAIL;

		dx11::dxgifactory factory;
		result = this->graphic_->adapter->GetParent(__uuidof(dx11::idxgifactory), (void**)factory.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed get factory from adapter");
			return E_FAIL;
		}
		stx::logger::Info("suceeded get factory from adapter");

		//detach DirectX Message Que from monitering of DXGI
		result = factory->MakeWindowAssociation(this->window_, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(result))
		{
			stx::logger::Error("failed make window association");
			return E_FAIL;
		}
		stx::logger::Error("succeeded make window association");

		return S_OK;
	}

	inline win32::result Graphics::SetDefaultRasterizeMethod()
	{
		stx::logger::scope_message<stx::logger::start_complete> msg("Set Default Rasterize Method");

		auto window = win32::WindowManager::GetInstance()->GetCurrentWindow<GraphicWindowPtr>();
		auto cg = window->GetCommonGraphics();
		
		auto rasterizerstate = cg->CreateRasterizerState(
			D3D11_FILL_SOLID,
			D3D11_CULL_BACK,
			FALSE,
			0,
			0,
			0,
			TRUE,
			FALSE);

		if (!rasterizerstate.Get())return E_FAIL;

		this->graphic_->devicecontext->RSSetState(rasterizerstate.Get());

		return S_OK;

	}

	inline win32::result Graphics::SetDefaultDepthStencilState()
	{
		stx::logger::scope_message<stx::logger::start_complete> msg("Set Default Dpeth Stencil State");

		win32::result result = E_FAIL;

		dx11::depthstencilstate depthstencilstate;
		dx11::depthstencil_desc depthstencil_desc;

		ZeroMemory(&depthstencil_desc, sizeof(depthstencil_desc));

		depthstencil_desc.DepthEnable = true;
		depthstencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthstencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depthstencil_desc.StencilEnable = false;

		//Create Depth Stencil State
		result = this->graphic_->device->CreateDepthStencilState(&depthstencil_desc, depthstencilstate.ToCreator());
		if (FAILED(result))
		{
			stx::logger::Error("failed create depth stencil state");
			return E_FAIL;
		}
		stx::logger::Info("succeeded create depth stencil state");

		this->graphic_->devicecontext->OMSetDepthStencilState(depthstencilstate.Get(), 0);

		return S_OK;

	}

	inline win32::result Graphics::OptimizeWindowMode(const uint2 size)
	{
		stx::logger::scope_message<stx::logger::start_complete> msg("Optimize WindowMode");

		win32::result result = E_FAIL;

		if (this->graphic_->swapchain == nullptr)
		{
			stx::logger::Error("swap chain is empty");
			return E_FAIL;
		}

		dx11::swapchain_desc swapchain_desc;
		result = this->graphic_->swapchain->GetDesc(&swapchain_desc);
		if (FAILED(result))
		{
			stx::logger::Error("failed get swapchain desc");
			return E_FAIL;
		}
		stx::logger::Info("succeeded get swapchain desc");

		//detach rendertargetview
		this->graphic_->devicecontext->OMSetRenderTargets(0, nullptr, nullptr);

		//release
		this->graphic_->rendertargetview = nullptr;
		this->graphic_->depthstencilview = nullptr;
		stx::logger::Info("Release Terget Views");

		if (swapchain_desc.BufferDesc.Height != size.x ||
			swapchain_desc.BufferDesc.Width != size.y)
		{
			//refresh buffer
			result = this->graphic_->swapchain->ResizeBuffers(
				swapchain_desc.BufferCount,
				size.x,
				size.y,
				swapchain_desc.BufferDesc.Format,
				swapchain_desc.Flags);
			if (FAILED(result))
			{
				stx::logger::Error("failed resize swap chain back buffer size and format");
				return E_FAIL;
			}
			stx::logger::Info("succeeded resize swap chain back buffer size and format");
		}

		if (FAILED(this->CreateRenderTargetView()))return E_FAIL;
		if (FAILED(this->CreateDepthStencilView()))return E_FAIL;

		//reconcile alignment for explicit release
		this->graphic_->rendertargetview->AddRef();
		this->graphic_->depthstencilview->AddRef();

		this->graphic_->devicecontext->OMSetRenderTargets(1, &this->graphic_->rendertargetview, this->graphic_->depthstencilview.Get());

		return S_OK;

	}


	win32::result Graphics::ChangeWindowMode(bool resize)
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("Change WindowMode");

		win32::result result = E_FAIL;

		if (this->graphic_->swapchain == nullptr)
		{
			stx::logger::Error("swap chain is empty");
			return E_FAIL;
		}

		dx11::swapchain_desc swapchain_desc;
		result = this->graphic_->swapchain->GetDesc(&swapchain_desc);
		if (FAILED(result))
		{
			stx::logger::Error("failed get swapchain desc");
			return E_FAIL;
		}
		stx::logger::Info("succeeded get swapchain desc");

		BOOL fullscreen_now;

		//Get Fullscreen State
		result = this->graphic_->swapchain->GetFullscreenState(&fullscreen_now, nullptr);
		if (FAILED(result))
		{
			stx::logger::Error("failed get screen state");
			return E_FAIL;
		}
		stx::logger::Error("succeeded get screen state");


		result = this->graphic_->swapchain->SetFullscreenState(!fullscreen_now, nullptr);
		if (FAILED(result))
		{
			stx::logger::Error("failed set screen state");
			return E_FAIL;
		}
		stx::logger::Error("succeeded set screen state");

		if (fullscreen_now)
		{
			stx::logger::Info("to window mode");
			ShowWindow(this->window_, SW_SHOW);
			result = this->OptimizeWindowMode({ 0, 0 });
		} else
		{
			stx::logger::Info("to fullscreen mode");
		}

		return S_OK;
	}

	dx11::displaymode_desc* Graphics::FindFitDesc(float2 size)
	{
		if (this->displaymodesdesclist_.empty())return nullptr;

		//UNSPECIFIEDがあったら消す　邪魔
		auto remove_when = [](dx11::displaymode_desc &desc){return desc.Scaling == DXGI_MODE_SCALING_UNSPECIFIED; };
		auto &d = this->displaymodesdesclist_;
		d.erase(std::remove_if(d.begin(), d.end(), remove_when), d.end());

		int refidx = 0;
		for (int idx : stx::range((int)this->displaymodesdesclist_.size() - 1, 0))
		{
			if (this->displaymodesdesclist_[idx].Width == size.x &&
				this->displaymodesdesclist_[idx].Height == size.y)
			{
				refidx = idx;
				break;
			}
		}
		if (refidx == 0)
		{
			//ちょうどいいサイズがなかった時用
			for (int idx : stx::range((int)this->displaymodesdesclist_.size() - 2, 0))
			{
				if (this->displaymodesdesclist_[idx].Width < size.x &&
					this->displaymodesdesclist_[idx].Height < size.y)
				{
					refidx = idx + 1;
					break;
				}
			}
		}

		return &this->displaymodesdesclist_[refidx];
	}

};