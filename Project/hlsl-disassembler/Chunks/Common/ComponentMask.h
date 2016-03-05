#pragma once



namespace HLSLDisassembler
{
	enum class ComponentMask
	{
		None = 0,

		X = 1,
		Y = 2,
		Z = 4,
		W = 8,

		R = 1,
		G = 2,
		B = 4,
		A = 8,

		XY  = X | Y,
		XYZ = X | Y | Z,
		XYZW= X | Y | Z | W,
		All = X | Y | Z | W // 15
	};

	std::string ToStringShex(ComponentMask value);
};