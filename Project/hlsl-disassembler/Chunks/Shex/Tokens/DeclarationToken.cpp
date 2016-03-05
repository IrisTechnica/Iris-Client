
#include "DeclarationToken.h"

#include "ConstantBufferDeclarationToken.h"
#include "ControlPointCountDeclarationToken.h"
#include "FunctionBodyDeclarationToken.h"
#include "FunctionTableDeclarationToken.h"
#include "GeometryShaderInputPrimitiveDeclarationToken.h"
#include "GeometryShaderInstanceCountDeclarationToken.h"
#include "GeometryShaderMaxOutputVertexCountDeclarationToken.h"
#include "GeometryShaderOutputPrimitiveTopologyDeclarationToken.h"
#include "GlobalFlagsDeclarationToken.h"
#include "HullShaderForkPhaseInstanceCountDeclarationToken.h"
#include "HullShaderMaxTessFactorDeclarationToken.h"
#include "IndexableTempRegisterDeclarationToken.h"
#include "IndexingRangeDeclarationToken.h"
#include "InputRegisterDeclarationToken.h"
#include "InterfaceDeclarationToken.h"
#include "OutputRegisterDeclarationToken.h"
#include "RawShaderResourceViewDeclarationToken.h"
#include "RawThreadGroupSharedMemoryDeclarationToken.h"
#include "RawUnorderedAccessViewDeclarationToken.h"
#include "ResourceDeclarationToken.h"
#include "SamplerDeclarationToken.h"
#include "StructuredShaderResourceViewDeclarationToken.h"
#include "StructuredThreadGroupSharedMemoryDeclarationToken.h"
#include "StructuredUnorderedAccessViewDeclarationToken.h"
#include "TempRegisterDeclarationToken.h"
#include "TessellatorDomainDeclarationToken.h"
#include "TessellatorOutputPrimitiveDeclarationToken.h"
#include "TessellatorPartitioningDeclarationToken.h"
#include "ThreadGroupDeclarationToken.h"
#include "TypedUnorderedAccessViewDeclarationToken.h"

using namespace std;
using namespace HLSLDisassembler;

shared_ptr<DeclarationToken> DeclarationToken::Parse(BytecodeReader& reader, OpcodeType opcodeType)
{
	switch (opcodeType)
	{
	case OpcodeType::DclGlobalFlags:
		return GlobalFlagsDeclarationToken::Parse(reader);
	case OpcodeType::DclResource:
		return ResourceDeclarationToken::Parse(reader);
	case OpcodeType::DclSampler:
		return SamplerDeclarationToken::Parse(reader);
	case OpcodeType::DclInput:
	case OpcodeType::DclInputSgv:
	case OpcodeType::DclInputSiv:
	case OpcodeType::DclInputPs:
	case OpcodeType::DclInputPsSgv:
	case OpcodeType::DclInputPsSiv:
		return InputRegisterDeclarationToken::Parse(reader);
	case OpcodeType::DclOutput:
	case OpcodeType::DclOutputSgv:
	case OpcodeType::DclOutputSiv:
		return OutputRegisterDeclarationToken::Parse(reader);
	case OpcodeType::DclIndexRange:
		return IndexingRangeDeclarationToken::Parse(reader);
	case OpcodeType::DclTemps:
		return TempRegisterDeclarationToken::Parse(reader);
	case OpcodeType::DclIndexableTemp:
		return IndexableTempRegisterDeclarationToken::Parse(reader);
	case OpcodeType::DclConstantBuffer:
		return ConstantBufferDeclarationToken::Parse(reader);
	case OpcodeType::DclGsInputPrimitive:
		return GeometryShaderInputPrimitiveDeclarationToken::Parse(reader);
	case OpcodeType::DclGsOutputPrimitiveTopology:
		return GeometryShaderOutputPrimitiveTopologyDeclarationToken::Parse(reader);
	case OpcodeType::DclMaxOutputVertexCount:
		return GeometryShaderMaxOutputVertexCountDeclarationToken::Parse(reader);
	case OpcodeType::DclGsInstanceCount:
		return GeometryShaderInstanceCountDeclarationToken::Parse(reader);
	case OpcodeType::DclInputControlPointCount:
	case OpcodeType::DclOutputControlPointCount:
		return ControlPointCountDeclarationToken::Parse(reader);
	case OpcodeType::DclTessDomain:
		return TessellatorDomainDeclarationToken::Parse(reader);
	case OpcodeType::DclTessPartitioning:
		return TessellatorPartitioningDeclarationToken::Parse(reader);
	case OpcodeType::DclTessOutputPrimitive:
		return TessellatorOutputPrimitiveDeclarationToken::Parse(reader);
	case OpcodeType::DclHsMaxTessFactor:
		return HullShaderMaxTessFactorDeclarationToken::Parse(reader);
	case OpcodeType::DclHsForkPhaseInstanceCount:
		return HullShaderForkPhaseInstanceCountDeclarationToken::Parse(reader);
	case OpcodeType::DclFunctionBody :
		return FunctionBodyDeclarationToken::Parse(reader);
	case OpcodeType::DclFunctionTable :
		return FunctionTableDeclarationToken::Parse(reader);
	case OpcodeType::DclInterface :
		return InterfaceDeclarationToken::Parse(reader);
	case OpcodeType::DclThreadGroup:
		return ThreadGroupDeclarationToken::Parse(reader);
	case OpcodeType::DclUnorderedAccessViewTyped :
		return TypedUnorderedAccessViewDeclarationToken::Parse(reader);
	case OpcodeType::DclUnorderedAccessViewRaw :
		return RawUnorderedAccessViewDeclarationToken::Parse(reader);
	case OpcodeType::DclUnorderedAccessViewStructured :
		return StructuredUnorderedAccessViewDeclarationToken::Parse(reader);
	case OpcodeType::DclThreadGroupSharedMemoryRaw :
		return RawThreadGroupSharedMemoryDeclarationToken::Parse(reader);
	case OpcodeType::DclThreadGroupSharedMemoryStructured :
		return StructuredThreadGroupSharedMemoryDeclarationToken::Parse(reader);
	case OpcodeType::DclResourceRaw :
		return RawShaderResourceViewDeclarationToken::Parse(reader);
	case OpcodeType::DclResourceStructured:
		return StructuredShaderResourceViewDeclarationToken::Parse(reader);
	default:
		throw runtime_error("OpcodeType '" + to_string((int) opcodeType) + "' is not supported.");
	}
}

const Operand& DeclarationToken::GetOperand() const { return _operand; }

DeclarationToken::DeclarationToken(Operand operand)
	: _operand(operand)
{

}