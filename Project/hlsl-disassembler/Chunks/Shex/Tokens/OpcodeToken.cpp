
#include "OpcodeToken.h"

using namespace std;
using namespace HLSLDisassembler;

const OpcodeHeader& OpcodeToken::GetHeader() const { return _header; }
void OpcodeToken::SetHeader(OpcodeHeader header) { _header = header; }

ostream& HLSLDisassembler::operator<<(ostream& out, const OpcodeToken& value)
{
	value.Print(out);
	return out;
}

string OpcodeToken::GetTypeDescription() const
{
	return ToString(_header.OpcodeType);
}

void OpcodeToken::Print(std::ostream& out) const
{
	out << GetTypeDescription();
}