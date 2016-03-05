
#include "ImmediateDeclarationToken.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

uint32_t ImmediateDeclarationToken::GetDeclarationLength() const { return _declarationLength; }