
#include "NameToken.h"

#include "../../../Util/Decoder.h"

using namespace HLSLDisassembler;

SystemValueName NameToken::Parse(BytecodeReader& reader)
{
	auto token = reader.ReadUInt32();
	return DecodeValue<SystemValueName>(token, 0, 15);
}