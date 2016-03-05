#pragma once


#include "../DxbcChunk.h"

namespace HLSLDisassembler
{
	/// <summary>
	/// TODO
	/// Contains the original source code of the shader, and links source code lines to ASM ops.
	/// </summary>
	class DebuggingChunk : public DxbcChunk
	{
	public :
		static std::shared_ptr<DebuggingChunk> Parse(BytecodeReader& reader);
	};
};