#pragma once
#include "typetraits.hpp"

namespace stx
{
	template<class _Conditional>
	class require : compile_error<_Conditional::value>
	{};
};