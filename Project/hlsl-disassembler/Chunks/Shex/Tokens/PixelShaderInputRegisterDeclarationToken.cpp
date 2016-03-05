
#include "PixelShaderInputRegisterDeclarationToken.h"

#include "../../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

InterpolationMode PixelShaderInputRegisterDeclarationToken::GetInterpolationMode() const { return _interpolationMode; }

void PixelShaderInputRegisterDeclarationToken::Print(ostream& out) const
{
	out << GetTypeDescription() << " "
		<< ((_interpolationMode != InterpolationMode::Constant) ? ToString(_interpolationMode) + " " : "")
		<< GetOperand();

	if (GetHeader().OpcodeType == OpcodeType::DclInputPsSgv || GetHeader().OpcodeType == OpcodeType::DclInputPsSiv)
		out << ", " << ToString(GetSystemValueName());
}

PixelShaderInputRegisterDeclarationToken::PixelShaderInputRegisterDeclarationToken(InterpolationMode interpolationMode, Operand operand)
	: _interpolationMode(interpolationMode), InputRegisterDeclarationToken(operand)
{

}