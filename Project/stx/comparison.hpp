#pragma once
#include <cstdarg>

namespace stx
{
	template<class _Ty>
	class comparison
	{
	public:
		typedef comparison<_Ty> this_type;
	private:
		const _Ty& target_;
		bool result_;
	public:
		explicit comparison(const _Ty& target) :
			target_(target),
			result_(false)
		{};

		this_type& operator||(const _Ty& right)
		{
			result_ |= target_ || right;
			return *this;
		}

		this_type& operator&&(const _Ty& right)
		{
			result_ = target_ && right;
			return *this;
		}

		this_type& operator!()
		{
			result_ = !result_;
			return *this;
		}

		operator bool()
		{
			return result_;
		}
	};
};