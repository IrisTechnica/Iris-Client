#pragma once
#include <boost/detail/select_type.hpp>
#include "typetraits.hpp"

namespace stx { namespace detail {



	template<class _Enum>
	struct switch_types {
		template<
			bool _Check,
			size_t _N,
			_Enum _Select,
			_Enum _First,
			_Enum ... _Modes
		>
		struct resolve_select
		{
			static size_t const value = _N - 1;
		};

		template<
			size_t _N,
			_Enum _Select,
			_Enum _First,
			_Enum ... _Modes
		>
		struct resolve_select<false, _N, _Select,_First,_Modes ...> :
			public resolve_select<_First == _Select, _N + 1, _Select, _Modes ...>{};

		template<
			size_t _N,
			class _Check,
			class ... _Remain>
		struct resolve_type : public resolve_type<_N - 1, _Remain ... > {};

		template<
			class _Check,
			class ... _Remain>
		struct resolve_type<0, _Check, _Remain ...>
		{
			typedef _Check type;
		};

		template<_Enum ... _Modes>
		struct which {
			template<_Enum _Select>
			struct select {
				template<class ... _Types>
				struct types {
					typedef typename resolve_type<resolve_select<false, 0, _Select, _Modes ..., _Select>::value, _Types ...>::type type;
				};
			};
		};

	};


};};