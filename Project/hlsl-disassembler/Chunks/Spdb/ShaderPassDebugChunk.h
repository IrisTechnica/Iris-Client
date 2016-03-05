#pragma once
#include "../DxbcChunk.h"
#include "../Common/ShaderVersion.h"

namespace HLSLDisassembler
{

	class ShaderPassDebugChunk : public DxbcChunk
	{
	public:
		static std::shared_ptr<ShaderPassDebugChunk> Parse(BytecodeReader& reader, uint32_t sizeInBytes);

		ShaderPassDebugChunk();
		~ShaderPassDebugChunk();
	};

};