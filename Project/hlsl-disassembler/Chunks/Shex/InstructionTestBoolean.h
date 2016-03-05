#pragma once



namespace HLSLDisassembler
{
	enum class InstructionTestBoolean
	{
		Zero = 0,
		NonZero = 1
	};

	std::string ToString(InstructionTestBoolean value);
};