#pragma once


#include "InputOutputSignatureChunk.h"

namespace HLSLDisassembler
{
	class InputSignatureChunk : public InputOutputSignatureChunk
	{
	protected :
		virtual std::string GetOutputDescription() const;
	};
};