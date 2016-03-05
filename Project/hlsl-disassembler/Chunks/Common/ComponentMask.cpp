
#include "ComponentMask.h"

#include "../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToStringShex(ComponentMask value)
{
	string result;

	if (HasFlag(value, ComponentMask::X))
		result += "x";
	if (HasFlag(value, ComponentMask::Y))
		result += "y";
	if (HasFlag(value, ComponentMask::Z))
		result += "z";
	if (HasFlag(value, ComponentMask::W))
		result += "w";
			
	return result;
}