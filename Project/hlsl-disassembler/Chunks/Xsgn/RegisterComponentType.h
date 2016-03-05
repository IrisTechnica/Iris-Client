#pragma once



namespace HLSLDisassembler
{
	enum class RegisterComponentType
	{
		Unknown = 0,
		UInt32 = 1,
		SInt32 = 2,
		Float32 = 3,
		UNorm8 = 4,
	};

	std::string ToString(RegisterComponentType value);
};