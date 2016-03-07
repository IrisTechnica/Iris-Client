#pragma once
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "enumerable.hpp"
namespace
{

	template<class Target, class Source>
	struct flexible_cast_entity
	{
		static Target do_cast(Source &src)
		{
			return boost::lexical_cast<Target>(src);
		};
	};

};

template<class Target, class Source>
Target flexible_cast(Source src)
{
	return flexible_cast_entity<Target, Source>::do_cast(src);
};

template <typename Target, typename Source>
inline Target flexible_cast(const enumerable4<Source> &arg)
{
	return (boost::tformat(_T("%1% %2% %3% %4%")) % arg.x % arg.y % arg.z % arg.w).str();
}

template <class Target, class Source>
struct flexible_cast_entity < enumerable4<Target>, Source >
{
	static enumerable4<Target> do_cast(Source &src)
	{
		std::vector<tstring> results;
		boost::algorithm::split(results, boost::lexical_cast<tstring>(src), boost::is_any_of(" "));
		if (results.empty())throw boost::bad_lexical_cast();
		enumerable4<Target> ret = {
			boost::lexical_cast<Target>(results[0]),
			boost::lexical_cast<Target>(results[1]),
			boost::lexical_cast<Target>(results[2]),
			boost::lexical_cast<Target>(results[3])
		};
		return ret;
	}

};

template <typename Target, typename Source>
inline Target flexible_cast(const enumerable3<Source> &arg)
{
	return (boost::format("%1% %2% %3%") % arg.x % arg.y % arg.z).str();
}

template <class Target, class Source>
struct flexible_cast_entity < enumerable3<Target>, Source >
{
	static enumerable3<Target> do_cast(Source &src)
	{
		std::vector<tstring> results;
		boost::algorithm::split(results, boost::lexical_cast<tstring>(src), boost::is_any_of(" "));
		if (results.empty())throw boost::bad_lexical_cast();
		enumerable3<Target> ret = {
			boost::lexical_cast<Target>(results[0]),
			boost::lexical_cast<Target>(results[1]),
			boost::lexical_cast<Target>(results[2])
		};
		return ret;
	}

};

