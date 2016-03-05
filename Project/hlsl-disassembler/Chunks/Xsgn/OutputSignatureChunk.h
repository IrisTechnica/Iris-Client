#pragma once


#include "InputOutputSignatureChunk.h"

namespace HLSLDisassembler
{
	class OutputSignatureChunk : public InputOutputSignatureChunk
	{
	protected :
		virtual std::string GetOutputDescription() const;
	};
};