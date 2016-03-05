#pragma once
#include <d3d11shader.h>
#include <dxgi.h>
#include "../Chunks/Common/ComponentMask.h"
#include "../Chunks/Xsgn/RegisterComponentType.h"

namespace HLSLDisassembler
{

	DXGI_FORMAT GetDXGIFormat(RegisterComponentType type, ComponentMask mask);

	struct FormatDesc
	{
		DXGI_FORMAT format;
		RegisterComponentType type;
		ComponentMask mask;
	};

	static FormatDesc FormatTable[] =
	{
	{ DXGI_FORMAT_R32G32B32A32_FLOAT,	RegisterComponentType::Float32,	ComponentMask::XYZW,	},
	{ DXGI_FORMAT_R32G32B32A32_SINT,	RegisterComponentType::SInt32,	ComponentMask::XYZW,	},
	{ DXGI_FORMAT_R32G32B32A32_UINT,	RegisterComponentType::UInt32,	ComponentMask::XYZW,	},
	{ DXGI_FORMAT_R32G32B32_FLOAT,		RegisterComponentType::Float32,	ComponentMask::XYZ,		},
	{ DXGI_FORMAT_R32G32B32_SINT,		RegisterComponentType::SInt32,	ComponentMask::XYZ,		},
	{ DXGI_FORMAT_R32G32B32_UINT,		RegisterComponentType::UInt32,	ComponentMask::XYZ,		},
	{ DXGI_FORMAT_R32G32_FLOAT,			RegisterComponentType::Float32,	ComponentMask::XY,		},
	{ DXGI_FORMAT_R32G32_SINT,			RegisterComponentType::SInt32,	ComponentMask::XY,		},
	{ DXGI_FORMAT_R32G32_UINT,			RegisterComponentType::UInt32,	ComponentMask::XY,		},
	{ DXGI_FORMAT_R32_FLOAT,			RegisterComponentType::Float32,	ComponentMask::X,		},
	{ DXGI_FORMAT_R32_SINT,				RegisterComponentType::SInt32,	ComponentMask::X,		},
	{ DXGI_FORMAT_R32_UINT,				RegisterComponentType::UInt32,	ComponentMask::X,		},
	{ DXGI_FORMAT_R8G8B8A8_UNORM,		RegisterComponentType::UNorm8,	ComponentMask::XYZW,	},
	};
};