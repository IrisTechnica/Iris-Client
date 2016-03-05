#pragma once
#include <boolinq/boolinq.h>
#include "DXGIFormat.h"
#include <vector>

using namespace HLSLDisassembler;
using namespace boolinq;


DXGI_FORMAT HLSLDisassembler::GetDXGIFormat(RegisterComponentType type, ComponentMask mask)
{
	auto result = 
		from(FormatTable)
		.where([&](const FormatDesc& cmp){return cmp.type == type; })
		.where([&](const FormatDesc& cmp) {return cmp.mask == mask; })
		.select([](const FormatDesc& cmp)->DXGI_FORMAT {return cmp.format; }).toVector();
	if(result.empty())return DXGI_FORMAT_UNKNOWN;
	return result[0];
	
}

