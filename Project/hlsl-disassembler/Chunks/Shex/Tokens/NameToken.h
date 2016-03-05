#pragma once


#include "../../../Util/BytecodeReader.h"
#include "../SystemValueName.h"

namespace HLSLDisassembler
{
	/// <summary>
	/// Name Token (NameToken) (used in declaration statements)
	///
	/// [15:00] D3D10_SB_NAME enumeration
	/// [31:16] Reserved, 0
	/// </summary>
	class NameToken
	{
	public :
		static SystemValueName Parse(BytecodeReader& reader);
	};
};