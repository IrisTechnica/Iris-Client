
#include "DxbcContainerHeader.h"

using namespace HLSLDisassembler;

DxbcContainerHeader DxbcContainerHeader::Parse(BytecodeReader& reader)
{
	auto fourCc = reader.ReadUInt32();
	if (fourCc != 'CBXD')
		throw new std::runtime_error("Invalid FourCC");

	DxbcContainerHeader result;
	result._fourCc = fourCc;
	result._uniqueKey[0] = reader.ReadUInt32();
	result._uniqueKey[1] = reader.ReadUInt32();
	result._uniqueKey[2] = reader.ReadUInt32();
	result._uniqueKey[3] = reader.ReadUInt32();
	result._one = reader.ReadUInt32();
	result._totalSize = reader.ReadUInt32();
	result._chunkCount = reader.ReadUInt32();
	
	return result;
}

uint32_t DxbcContainerHeader::GetFourCc()
{
	return _fourCc;
}

uint32_t* DxbcContainerHeader::GetUniqueKey()
{
	return _uniqueKey;
}

uint32_t DxbcContainerHeader::GetOne()
{
	return _one;
}

uint32_t DxbcContainerHeader::GetTotalSize()
{
	return _totalSize;
}

uint32_t DxbcContainerHeader::GetChunkCount()
{
	return _chunkCount;
}