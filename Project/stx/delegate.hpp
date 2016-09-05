#pragma once
#include <functional>
#include <deque>
#include <stx/typetraits/typetraits.hpp>
#include <stx/typetraits/function_traits.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/equal.hpp>

#include <iostream>

namespace stx
{
	class delegate
	{
	public:

		using func_type	= std::function<void()>;
		using key_type	= unsigned long;
		using pair_type	= std::pair<key_type, func_type>;
		using que_type = std::deque<pair_type>;

	public:

		template<class func
			, class translator = stx::to_function<func>
			, class function_type = translator::type
			, typename boost::enable_if<boost::mpl::equal<func_type, function_type>>::type *& = stx::enabler
		>
		const key_type operator+=(const func &data)
		{
			function_type func = data;
			auto key = get_hash(data);
			que.push_back(std::make_pair(key, data));
			return key;
		}

		void operator()()
		{
			for (const auto &event : que)
			{
				event.second();
			}
		}

	private:
		template<class func, typename std::enable_if<std::is_pointer<func>::value>::type *& = stx::enabler>
		const key_type get_hash(const func &data)
		{
			return (key_type)data;
		}

		template<class func, typename stx::disable_if<std::is_pointer<func>::value>::type *& = stx::enabler>
		const key_type get_hash(const func &data)
		{
			return (key_type)&data;
		}

		que_type que;
	};
};