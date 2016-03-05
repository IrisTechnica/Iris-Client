#pragma once



namespace HLSLDisassembler
{
	enum class ConstantBufferAccessPattern
	{
		ImmediateIndexed = 0,
		DynamicIndexed = 1
	};

	std::string ToString(ConstantBufferAccessPattern value);
};