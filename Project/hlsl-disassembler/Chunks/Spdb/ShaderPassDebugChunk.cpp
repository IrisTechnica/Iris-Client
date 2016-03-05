#include "ShaderPassDebugChunk.h"

using namespace HLSLDisassembler;

std::shared_ptr<ShaderPassDebugChunk> HLSLDisassembler::ShaderPassDebugChunk::Parse(BytecodeReader & reader, uint32_t sizeInBytes)
{
	return std::shared_ptr<ShaderPassDebugChunk>(new ShaderPassDebugChunk());
}

ShaderPassDebugChunk::ShaderPassDebugChunk()
{
}


ShaderPassDebugChunk::~ShaderPassDebugChunk()
{
}
