
#include "ConstantBufferDeclarationToken.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<ConstantBufferDeclarationToken> ConstantBufferDeclarationToken::Parse(BytecodeReader& reader)
{
	auto token0 = reader.ReadUInt32();
	auto accessPattern = DecodeValue<ConstantBufferAccessPattern>(token0, 11, 11);
	auto operand = Operand::Parse(reader, DecodeValue<OpcodeType>(token0, 0, 10));
	auto result = shared_ptr<ConstantBufferDeclarationToken>(new ConstantBufferDeclarationToken(operand));
	result->_accessPattern = accessPattern;
	return result;
}

ConstantBufferAccessPattern ConstantBufferDeclarationToken::GetAccessPattern() const { return _accessPattern; }

void ConstantBufferDeclarationToken::Print(std::ostream& out) const
{
	out << GetTypeDescription() << " " << GetOperand() << ", " << ToString(_accessPattern);
}

ConstantBufferDeclarationToken::ConstantBufferDeclarationToken(Operand operand)
	: DeclarationToken(operand)
{

}