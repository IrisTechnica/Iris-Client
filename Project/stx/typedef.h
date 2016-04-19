#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

namespace boost
{
	namespace property_tree
	{
		typedef basic_ptree<tstring, tstring> tptree;
		typedef string_path<tstring, id_translator<tstring> > tpath;
	}
	typedef basic_format<TCHAR> tformat;
}

// std::cout fix

#if defined(_UNICODE)

#define tcin	wcin
#define tcout	wcout
#define tcerr	wcerr
#define tclog	wclog

#else

#define tcin	cin
#define tcout	cout
#define tcerr	cerr
#define tclog	clog

#endif

