
#include "OutputRegisterDeclarationToken.h"

#include "../../../Util/Decoder.h"
#include "NameToken.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<OutputRegisterDeclarationToken> OutputRegisterDeclarationToken::Parse(BytecodeReader& reader)
{
	auto token0 = reader.ReadUInt32();
	auto opcodeType = DecodeValue<OpcodeType>(token0, 0, 10);
	auto operand = Operand::Parse(reader, opcodeType);

	auto result = shared_ptr<OutputRegisterDeclarationToken>(new OutputRegisterDeclarationToken(operand));

	switch (opcodeType)
	{
	case OpcodeType::DclOutputSgv:
	case OpcodeType::DclOutputSiv:
		result->_systemValueName = NameToken::Parse(reader);
		break;
	}

	return result;
}

SystemValueName OutputRegisterDeclarationToken::GetSystemValueName() const { return _systemValueName; }

void OutputRegisterDeclarationToken::Print(ostream& out) const
{
	out << GetTypeDescription() << " " << GetOperand();

	if (GetHeader().OpcodeType == OpcodeType::DclOutputSgv || GetHeader().OpcodeType == OpcodeType::DclOutputSiv)
		out << ", " + ToString(_systemValueName);
}

OutputRegisterDeclarationToken::OutputRegisterDeclarationToken(Operand operand)
	: DeclarationToken(operand)
{

}