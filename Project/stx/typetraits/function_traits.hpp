#pragma once
#include "typetraits.hpp"
#include "lambda_traits.hpp"
#include <functional>

namespace stx
{
	namespace detail
	{
		namespace impl
		{
			template<class Null>
			struct to_function_impl
			{};

			template<class Rt, class ...Args>
			struct to_function_impl<Rt(Args...)>
			{
				static constexpr bool is_member_func = false;
				typedef typename std::function<Rt(Args...)> type;
				enum { arity = sizeof...(Args) };
				typedef void class_type;
			};

			template<class Rt, class Cls, class ...Args>
			struct to_function_impl<Rt(Cls::*)(Args...)> : public to_function_impl<Rt(Args...)>
			{
				static constexpr bool is_member_func = true;
				typedef Cls class_type;
			};
		}

		template<class Ld>
		struct to_function : public detail::lambda_type_inherit<Ld, impl::to_function_impl>
		{};

		template<class Rt, class ...Args>
		struct to_function<std::function<Rt(Args...)>> : public impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<std::function<Rt(*)(Args...)>> : public impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class Cls, class ...Args>
		struct to_function<std::function<Rt(Cls::*)(Args...)>> : public impl::to_function_impl<Rt(Cls::*)(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<Rt(Args...)> : public impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class ...Args>
		struct to_function<Rt(*)(Args...)> : public impl::to_function_impl<Rt(Args...)>
		{};

		template<class Rt, class Cls, class ...Args>
		struct to_function<Rt(Cls::*)(Args...)> : public impl::to_function_impl<Rt(Cls::*)(Args...)>
		{};

	}

	template<class Fc>
	struct to_function : 
		public detail::to_function<Fc>
	{};


};