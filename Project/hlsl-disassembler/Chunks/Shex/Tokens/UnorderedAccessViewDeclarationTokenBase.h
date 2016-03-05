#pragma once


#include "DeclarationToken.h"
#include "../UnorderedAccessViewCoherency.h"

namespace HLSLDisassembler
{
	class UnorderedAccessViewDeclarationTokenBase : public DeclarationToken
	{
	public :
		UnorderedAccessViewCoherency GetCoherency() const;

	protected :
		UnorderedAccessViewDeclarationTokenBase(UnorderedAccessViewCoherency coherency, Operand operand);

	private:
		const UnorderedAccessViewCoherency _coherency;
	};
};