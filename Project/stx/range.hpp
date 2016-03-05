#pragma once

#include <type_traits>
#include <cassert>

namespace stx
{

template <typename T> class Range
{
public:
	static_assert(std::is_integral<T>::value,
		"T must be a integer type");

	Range(const T &First, const T &Last)
		: first_(First), last_(Last), step_(1)
	{
		//assert(First <= Last);
		if (First > Last)step_ *= -1;
	}

	class Iterator
	{
	public:
		Iterator(T Position, T Step)
			: position_(Position), step_(Step) {}

		bool operator!=(const Iterator &Rhs)
		{
			if (step_ > 0)
				return position_ < Rhs.position_;
			else
				return position_ > Rhs.position_;
		}

		Iterator& operator++()
		{
			position_ += step_;

			return *this;
		}

		T& operator*() { return position_; }

	private:
		T position_;
		T step_;
	};

	Iterator begin() { return Iterator(first_, step_); }
	Iterator end() { return Iterator(last_, step_); }

	Range& step(const T &Step)
	{
		//assert(0 <= Step);
		assert(0 != Step);
		step_ = Step;
		return *this;
	}
private:
	T first_;
	T last_;
	T step_;
};

template <typename T> Range<T> range(T First, T Last)
{
	return Range<T>(First, Last);
}

template <typename T> Range<T> range(T Last)
{
	return Range<T>(0, Last);
}

};