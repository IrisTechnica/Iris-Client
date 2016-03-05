#pragma once

#include <unordered_map>
#include <functional>

#pragma warning( disable : 4290 )
#pragma warning( disable : 4101 )

namespace stx
{
	template<class _At, class _Ct>
	using apply_map = std::unordered_map < _At, std::function<void(_Ct&)>>;

	template<class _At, class _Ct>
	void apply(std::unordered_map<_At, _Ct>& placeholder, const std::unordered_map<_At, std::function<void(_Ct&)>>& applycator) throw(std::exception)
	{
		bool applied = false;
		for (auto &holder : placeholder)
		{
			try {
				applycator.at(holder.first)(holder.second);
				applied = true;
			}
			catch (std::exception& e)
			{
				continue;
			}
		}
		if (!applied)throw(std::exception("not applied."));
	}

	template<class _At, class _Ct>
	void apply(std::pair<_At, _Ct>& placeholder, const std::unordered_map<_At, std::function<void(_Ct&)>>& applycator) throw(std::exception)
	{
		std::unordered_map<_At, _Ct> map;
		map.insert(placeholder);
		apply(map, applycator);
	}

	template<class _At, class _Ct, class _Df>
	void apply_default(std::unordered_map<_At, _Ct>& placeholder, const std::unordered_map<_At, std::function<void(_Ct&)>>& applycator, _Df default_apply)
	{
		auto default_apply_impl = _At(default_apply);
		for (auto &holder : placeholder)
		{
			try {
				applycator.at(holder.first)(holder.second);
			}
			catch (std::exception& e)
			{
				applycator.at(default_apply_impl)(holder.second);
				continue;
			}
		}
	}

	template<class _At, class _Ct, class _Df>
	void apply_default(std::pair<_At, _Ct>& placeholder, const std::unordered_map<_At, std::function<void(_Ct&)>>& applycator, _Df default_apply)
	{
		std::unordered_map<_At, _Ct> map;
		map.insert(placeholder);
		apply_default(map, applycator, default_apply);
	}
};


#pragma warning( default : 4290 )
#pragma warning( default : 4101 )
