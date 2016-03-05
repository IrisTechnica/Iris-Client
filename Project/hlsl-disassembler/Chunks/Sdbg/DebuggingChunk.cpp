
#include "DebuggingChunk.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<DebuggingChunk> DebuggingChunk::Parse(BytecodeReader& reader)
{
	return shared_ptr<DebuggingChunk>(new DebuggingChunk());
}