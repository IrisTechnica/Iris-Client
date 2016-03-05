
#include "RawThreadGroupSharedMemoryDeclarationToken.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<RawThreadGroupSharedMemoryDeclarationToken> RawThreadGroupSharedMemoryDeclarationToken::Parse(BytecodeReader& reader)
{
	auto token0 = reader.ReadUInt32();
	auto operand = Operand::Parse(reader, DecodeValue<OpcodeType>(token0, 0, 10));
	auto result = shared_ptr<RawThreadGroupSharedMemoryDeclarationToken>(new RawThreadGroupSharedMemoryDeclarationToken(operand));
	result->_elementCount = reader.ReadUInt32();
	return result;
}

uint32_t RawThreadGroupSharedMemoryDeclarationToken::GetElementCount() const { return _elementCount; }

void RawThreadGroupSharedMemoryDeclarationToken::Print(ostream& out) const
{
	out << GetTypeDescription() << " " << GetOperand();
}

RawThreadGroupSharedMemoryDeclarationToken::RawThreadGroupSharedMemoryDeclarationToken(Operand operand)
	: DeclarationToken(operand)
{

}