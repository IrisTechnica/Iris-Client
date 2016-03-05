#pragma once
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp> // is_any_of
#include "filesystem.h"

namespace stx
{
	namespace filesystem
	{
		std::vector<fileinfo> get_fileinfo_from_folder(const tstring & path)
		{
			namespace fs = boost::filesystem;

			std::vector<fileinfo> ret;
			auto it = fs::directory_iterator(path);

			for (auto stat : it)
			{
				if (!fs::is_directory(stat))
				{
					fileinfo info;
					auto filepath = stat.path();
					auto timestamp = boost::posix_time::from_time_t(fs::last_write_time(filepath));

					static std::locale loc(std::wcout.getloc(), new boost::posix_time::wtime_facet(L"%Y”N%mŒŽ%d“ú %H:%M:%S"));
					std::wstringstream wss;
					wss.imbue(loc);
					wss << timestamp;

					info.timestamp = unicode::ToTString(wss.str());
					info.size = fs::file_size(filepath);
					info.name = filepath.filename().string();
					info.path = filepath;
					info.extension = filepath.filename().extension().string();

					ret.push_back(info);
				}
			}
			return ret;
		}

		tstring get_fileextension(const tstring& filename)
		{
			std::vector<tstring> result;
			boost::algorithm::split(result, filename, boost::is_any_of("."));
			return result.empty() ? "" : *(result.end() - 1);
		}

		const boost::filesystem::path& safe_path(const boost::filesystem::path & path)
		{
			namespace fs = boost::filesystem;
			if (!fs::exists(path) && !path.has_extension())
			{
				fs::create_directory(path);
			}
			return path;
		}

	};
};

