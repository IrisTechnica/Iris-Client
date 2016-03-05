#pragma once



namespace HLSLDisassembler
{
	enum class ExtendedOperandType
	{
		/// <summary>
		/// Might be used if this enum is full and further extended opcode is needed.
		/// </summary>
		Empty = 0, 

		Modifier = 1,
	};
};