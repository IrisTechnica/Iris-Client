
#include "UnorderedAccessViewDeclarationTokenBase.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

UnorderedAccessViewCoherency UnorderedAccessViewDeclarationTokenBase::GetCoherency() const { return _coherency; }

UnorderedAccessViewDeclarationTokenBase::UnorderedAccessViewDeclarationTokenBase(UnorderedAccessViewCoherency coherency, Operand operand)
	: _coherency(coherency), DeclarationToken(operand)
{

}