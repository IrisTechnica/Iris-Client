
#include "GlobalFlags.h"

#include "../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

void AppendGlobalFlag(string& result, string value)
{
	if (!result.empty())
		result += " | ";
	result += value;
}

string HLSLDisassembler::ToString(GlobalFlags value)
{
	string result;

	if (HasFlag(value, GlobalFlags::RefactoringAllowed))
		AppendGlobalFlag(result, "refactoringAllowed");
	if (HasFlag(value, GlobalFlags::EnableRawAndStructuredBuffersInNonCsShaders))
		AppendGlobalFlag(result, "enableRawAndStructuredBuffers");

	return result;
}