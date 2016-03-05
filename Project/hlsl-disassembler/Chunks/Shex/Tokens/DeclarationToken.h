#pragma once


#include "../../../Util/BytecodeReader.h"
#include "OpcodeToken.h"
#include "Operand.h"

namespace HLSLDisassembler
{
	class DeclarationToken : public OpcodeToken
	{
	public :
		static std::shared_ptr<DeclarationToken> Parse(BytecodeReader& reader, OpcodeType opcodeType);

		const Operand& GetOperand() const;

	protected :
		DeclarationToken(Operand operand);
		DeclarationToken() { }

	private :
		const Operand _operand;
	};
};