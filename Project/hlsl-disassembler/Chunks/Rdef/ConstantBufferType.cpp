
#include "ConstantBufferType.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToString(ConstantBufferType value)
{
	switch (value)
	{
	case ConstantBufferType::ConstantBuffer :
		return "cbuffer";
	case ConstantBufferType::TextureBuffer :
		return "tbuffer";
	case ConstantBufferType::InterfacePointers :
		return "interfaces";
	case ConstantBufferType::ResourceBindInformation :
		return "Resource bind info for";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}