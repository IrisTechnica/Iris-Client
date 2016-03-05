
#include "InstructionTestBoolean.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToString(InstructionTestBoolean value)
{
	switch (value)
	{
	case InstructionTestBoolean::Zero :
		return "z";
	case InstructionTestBoolean::NonZero :
		return "nz";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}