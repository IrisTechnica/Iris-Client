
#include "SyncFlags.h"

#include "../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToString(SyncFlags value)
{
	string result;

	if (HasFlag(value, SyncFlags::UnorderedAccessViewGlobal))
		result += "_uglobal";
	if (HasFlag(value, SyncFlags::UnorderedAccessViewGroup))
		result += "_ugroup";
	if (HasFlag(value, SyncFlags::SharedMemory))
		result += "_g";
	if (HasFlag(value, SyncFlags::ThreadsInGroup))
		result += "_t";

	return result;
}