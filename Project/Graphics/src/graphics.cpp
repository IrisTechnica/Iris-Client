#include "graphics.h"
#include <stx/com_ptr.h>
#include <stx/range.hpp>
#include <stx/logger.hpp>
#include <stx/logger/failchecker.hpp>

//external include
#include <dxgi.h>
#include <boost/range/adaptor/reversed.hpp>


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

	// create around display
	this->CreateAdaptor();
	this->InitDisplayMode(DXGI_FORMAT_R8G8B8A8_UNORM);

	auto matchdesc = this->FindFitDesc(window->GetClientSize());

	this->CreateDevice();
}

void Graphics::CreateAdaptor()
{
	stx::logger::scope_message<stx::logger::begin_end> msg(_T("Create Adapter"));

	win32::result result = E_FAIL;
	dx11::dxgifactory factory;

	if (this->adaptor == nullptr)
	{
		result = CreateDXGIFactory(__uuidof(dx11::idxgifactory), (void**)(factory.ToCreator()));
		stx::fail_checker::check(result, _T("create factory"));

		result = factory->EnumAdapters(0, &this->adaptor);
		stx::fail_checker::check(result, _T("create adaptor"));
	}

}

void Graphics::InitDisplayMode(const dx11::display_format& format)
{
	stx::logger::scope_message<stx::logger::begin_end> msg(_T("Init DisplayMode"));

	stx::com_ptr<IDXGIOutput> output;

	win32::result result = E_FAIL;

	displayFormat = format;

	{
		stx::logger::scope_message<stx::logger::start_complete> msg(_T("Enumerate of Display Mode"));

		// get primary display
		result = this->adaptor->EnumOutputs(0, &output);
		stx::fail_checker::check(result, _T("get primary display."));

		// enumrate of display mode
		unsigned displaymode_num;
		result = output->GetDisplayModeList(format, 0, &displaymode_num, nullptr);
		stx::fail_checker::check(result, _T("enumrate of display mode list."));

		this->desclist.resize(displaymode_num);
		result = output->GetDisplayModeList(format, 0, &displaymode_num, this->desclist.data());
		stx::fail_checker::check(result, _T("get display mode list"));

#ifdef _DEBUG
		for (auto& desc : this->desclist)
		{
			stx::logger::Debug(_T("Resolution [ %d : %d  @Rate[ %d / %d ] ]"),
				desc.Width,
				desc.Height,
				desc.RefreshRate.Denominator,
				desc.RefreshRate.Numerator);
		}
#endif
	}
}

const dx11::displaymode_desc& Graphics::FindFitDesc(float2 size)
{
	stx::fail_checker::check(!this->desclist.empty(), _T("desclist is empty."));

	{
		// if exist UNSPECIFIED mode, then remove it of the way.
		auto remove_when = [](dx11::displaymode_desc &desc) {return desc.Scaling == DXGI_MODE_SCALING_UNSPECIFIED; };
		this->desclist.erase(std::remove_if(this->desclist.begin(), this->desclist.end(), remove_when), this->desclist.end());
	}

	// now, find just size in first pass
	{
		stx::logger::scope_message<stx::logger::begin_end> msg(_T("Check First Pass"));

		for (auto& desc : boost::adaptors::reverse(this->desclist))
		{
			if (desc.Width == size.x && desc.Height == size.y)
				return desc;
		}
	}

	// then, find containable size in second pass, cannot found just size
	{
		stx::logger::scope_message<stx::logger::begin_end> msg(_T("Check Second Pass"));

		for (int idx : stx::range(static_cast<int>(this->desclist.size()) - 2, 0))
		{
			auto &desc = this->desclist[idx];
			if (desc.Width < size.x && desc.Height < size.y)
				return this->desclist[idx + 1];
		}
	}

	stx::logger::Error(_T("came to point that should not reach."));
	return dx11::displaymode_desc();
}

void Graphics::CreateDevice()
{
	stx::logger::scope_message<stx::logger::start_complete> msg(_T("Create Device"));

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
#ifdef _DEBUG
	device_flag = D3D11_CREATE_DEVICE_DEBUG;
	stx::logger::Debug(_T("use debug layer."));
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
		this->device.ToCreator(),
		&this->featureLevel,	//実際に使用されたモード
		this->deviceContext.ToCreator()
		);

	if (FAILED(result))
	{
		stx::logger::Info(_T("failed create lasted mode device. retry on auto mode."));
		//secondary initialize on directx9 to 10
		result = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			device_flag,
			featurelevel.data(),
			featurelevel.size(),	//自動モード
			D3D11_SDK_VERSION,
			this->device.ToCreator(),
			&this->featureLevel,	//実際に使用されたモード
			this->deviceContext.ToCreator()
			);
	}
	stx::fail_checker::check(result, _T("create device."));

#ifdef _DEBUG
	{
		stx::logger::Debug(_T("check used adapter type."));
		dx11::adapter_desc desc;
		this->adaptor->GetDesc(&desc);
		tstring str = unicode::ToTString(desc.Description);

		switch (this->featureLevel)
		{
		case D3D_FEATURE_LEVEL_11_1:
			stx::logger::Debug(_T("DirectX 11.1 SM(5) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_11_0:
			stx::logger::Debug(_T("DirectX 11.0 SM(5) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_10_1:
			stx::logger::Debug(_T("DirectX 10.1 SM(4) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_10_0:
			stx::logger::Debug(_T("DirectX 10.0 SM(4) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_9_3:
			stx::logger::Debug(_T("DirectX 9.3 SM(3) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_9_2:
			stx::logger::Debug(_T("DirectX 9.2 SM(2) %s"), str);
			break;
		case D3D_FEATURE_LEVEL_9_1:
			stx::logger::Debug(_T("DirectX 9.1 SM(1) %s"), str);
			break;
		}
	}
#endif

}

