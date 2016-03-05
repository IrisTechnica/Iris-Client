#pragma once

#include "../unicode.hpp"

#ifdef _WIN32
#include "win32/detail.h"
#endif

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/conversion.hpp>
#include <vector>

namespace stx
{
	namespace filesystem
	{

		struct fileinfo
		{
			boost::filesystem::path path;
			tstring name;
			tstring extension;
			size_t size;
			tstring timestamp;
		};

		std::vector<fileinfo> get_fileinfo_from_folder(const tstring& path);

		tstring get_fileextension(const tstring&);

		// make it before return, if donot exist path( path := directory )
		const boost::filesystem::path& safe_path(const boost::filesystem::path &path);

		static const std::wstring prohibition_regex = LR"(([E:;/B.,\)\]pAlrtvxz??‡fh??\(\[okqsuwy??‡€eg??]*)([^E:;/B.,\)\]pAlrtvxz??‡fh??\(\[okqsuwy??‡€eg??]*)([\(\[okqsuwy??‡€eg??]*))";
		static const std::wstring prohibition_regex_result = L"$2";
	};
};

