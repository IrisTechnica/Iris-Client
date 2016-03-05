#pragma once



namespace HLSLDisassembler
{
	enum class CustomDataClass
	{
		Comment = 0,
		DebugInfo,
		Opaque,
		DclImmediateConstantBuffer,
		ShaderMessage
	};
};