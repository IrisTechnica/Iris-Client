
#include "Sfi0Chunk.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<Sfi0Chunk> Sfi0Chunk::Parse(BytecodeReader& reader)
{
	auto unknown = reader.ReadInt32();
	assert(unknown == 2); // TODO: Unknown
	return shared_ptr<Sfi0Chunk>(new Sfi0Chunk());
}