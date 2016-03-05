#pragma once


#include "../DxbcChunk.h"
#include "Tokens/OpcodeToken.h"
#include "../Common/ShaderVersion.h"

namespace HLSLDisassembler
{
	class ShaderProgramChunk : public DxbcChunk
	{
	public :
		static std::shared_ptr<ShaderProgramChunk> Parse(BytecodeReader& reader);

		const ShaderVersion& GetVersion() const;
		uint32_t GetLength() const;
		const std::vector<std::shared_ptr<OpcodeToken>>& GetTokens() const;

		friend std::ostream& operator<<(std::ostream& out, const ShaderProgramChunk& value);

	private :
		ShaderProgramChunk(ShaderVersion version);

		const ShaderVersion _version;
		uint32_t _length;
		std::vector<std::shared_ptr<OpcodeToken>> _tokens;
	};
};