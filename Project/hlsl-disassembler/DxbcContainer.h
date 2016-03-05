#pragma once


#include "Util/BytecodeReader.h"
#include "DxbcContainerHeader.h"

#include "Fwd/raw_ptr.hpp"

namespace HLSLDisassembler
{
	class DxbcChunk;
	class ResourceDefinitionChunk;
	class PatchConstantSignatureChunk;
	class InputSignatureChunk;
	class OutputSignatureChunk;
	class ShaderProgramChunk;
	class StatisticsChunk;
	class InterfacesChunk;
	class ShaderPassDebugChunk;

	class DxbcContainer
	{
	public :
		static DxbcContainer Parse(const stx::filesystem::raw_ptr raw_bytes);
		static DxbcContainer Parse(const std::vector<char> bytes);
		static DxbcContainer Parse(BytecodeReader& reader);

		const std::shared_ptr<ResourceDefinitionChunk> GetResourceDefinition() const;
		const std::shared_ptr<PatchConstantSignatureChunk> GetPatchConstantSignature() const;
		const std::shared_ptr<InputSignatureChunk> GetInputSignature() const;
		const std::shared_ptr<OutputSignatureChunk> GetOutputSignature() const;
		const std::shared_ptr<ShaderProgramChunk> GetShader() const;
		const std::shared_ptr<StatisticsChunk> GetStatistics() const;
		const std::shared_ptr<InterfacesChunk> GetInterfaces() const;

		friend std::ostream& operator<<(std::ostream &out, const DxbcContainer &container);

	private :
		DxbcContainerHeader _header;
		std::vector<std::shared_ptr<DxbcChunk>> _chunks;
	};
};