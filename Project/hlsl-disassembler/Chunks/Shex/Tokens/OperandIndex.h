#pragma once



namespace HLSLDisassembler
{
	class Operand;

	struct OperandIndex
	{
	public :
		uint64_t Value;
		std::shared_ptr<Operand> Register;

		friend std::ostream& operator<<(std::ostream& out, const OperandIndex& value);
	};
};