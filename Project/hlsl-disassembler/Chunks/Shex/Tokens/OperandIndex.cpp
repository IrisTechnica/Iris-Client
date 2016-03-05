
#include "OperandIndex.h"

#include "Operand.h"

using namespace std;
using namespace HLSLDisassembler;

std::ostream& HLSLDisassembler::operator<<(std::ostream& out, const OperandIndex& value)
{
	if (value.Register != nullptr)
		out << *value.Register << " + ";
	out << value.Value;
	return out;
}