#pragma once


#include "OpcodeType.h"

namespace HLSLDisassembler
{
	struct OpcodeHeader
	{
	public :
		OpcodeType OpcodeType;
		uint32_t Length;
		bool IsExtended;
	};
};