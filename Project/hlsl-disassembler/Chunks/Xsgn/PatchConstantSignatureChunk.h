#pragma once


#include "InputOutputSignatureChunk.h"

namespace HLSLDisassembler
{
	class PatchConstantSignatureChunk : public InputOutputSignatureChunk
	{
	protected :
		virtual std::string GetOutputDescription() const;
	};
};