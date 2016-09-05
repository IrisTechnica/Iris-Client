#pragma once
#include "typetraits.hpp"
#include <boost/lambda/lambda.hpp>
#include <tuple>

namespace stx
{
	template<class _Ld>
	struct lambda_type
		: public lambda_type<decltype(&_Ld::operator())>
	{};

	template<class _Ret, class _Cls, class... _Args>
	struct lambda_type<_Ret(_Cls::*)(_Args...) const>
	{
		enum { arity = sizeof...(_Args) };

		typedef _Ret return_type;
		typedef _Cls class_type;

		template<size_t i>
		struct arg
		{
			typedef typename std::tuple_element<i, std::tuple<_Args...>>::type type;
		};
	};

	namespace detail
	{
		template<
			class _Ld,
			template<typename ...> class _Ih
		>
		struct lambda_type_inherit
			: public lambda_type_inherit<decltype(&_Ld::operator()),_Ih>
		{
			static constexpr bool is_lambda = true;
		};

		template<
			template<typename ...> class _Ih,
			class _Ret, class _Cls, class... _Args
		>
		struct lambda_type_inherit<_Ret(_Cls::*)(_Args...) const, _Ih> : public _Ih<_Ret(_Cls::*)(_Args...)>
		{
		};

		template<
			template<typename ...> class _Ih,
		class _Ret, class... _Args
		>
		struct lambda_type_inherit<_Ret(_Args...) const, _Ih> : public _Ih<_Ret(_Args...)>
		{
		};


	};
};