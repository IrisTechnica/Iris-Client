#pragma once


#include "CustomDataToken.h"

namespace HLSLDisassembler
{
	class ImmediateDeclarationToken : public CustomDataToken
	{
	public :
		uint32_t GetDeclarationLength() const;

	protected :
		uint32_t _declarationLength;
	};
};