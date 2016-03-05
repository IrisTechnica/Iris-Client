
#include "ControlPointCountDeclarationToken.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<ControlPointCountDeclarationToken> ControlPointCountDeclarationToken::Parse(BytecodeReader& reader)
{
	auto token0 = reader.ReadUInt32();
	auto result = shared_ptr<ControlPointCountDeclarationToken>(new ControlPointCountDeclarationToken());
	result->_controlPointCount = DecodeValue(token0, 11, 16);
	return result;
};

uint32_t ControlPointCountDeclarationToken::GetControlPointCount() const { return _controlPointCount; }

void ControlPointCountDeclarationToken::Print(std::ostream& out) const
{
	out << GetTypeDescription() << " " << _controlPointCount;
};