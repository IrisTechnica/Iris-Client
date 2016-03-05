#include "cache.h"
#include "../constant.hpp"
#include "../logger.hpp"

#include <boost/filesystem.hpp>

namespace stx
{
	namespace filesystem
	{
		namespace entity
		{
			cache::cache()
			{
			}


			cache::~cache()
			{
			}

			/**
			* FNV Constants
			*/
			static const uint32_t FNV_OFFSET_BASIS_32 = 2166136261U;
			static const uint64_t FNV_OFFSET_BASIS_64 = 14695981039346656037U;

			static const uint32_t FNV_PRIME_32 = 16777619U;
			static const uint64_t FNV_PRIME_64 = 1099511628211LLU;

			uint64_t cache::fnv_1_get_hash(const raw_ptr& data)
			{
				uint64_t hash;

				hash = FNV_OFFSET_BASIS_64;
				for (auto dat : *data) {
					hash = (FNV_PRIME_64 * hash) ^ (dat);
				}

				return hash;
			}

			raw_ptr cache::operator [](const tstring& filename)
			{
				auto size = boost::filesystem::file_size(filename);
				bool cache_enable = size < 1 * stx::gigabyte ? true : false;
				auto hash = stx::tstring_symbol(filename);


				if (cache_enable && path_map_.find(hash) != path_map_.end())
				{
					//exsit path map
					return resource_map_[path_map_[hash]];

				} else{
					rfstream ifs(filename, std::ios::binary);
					if (ifs.fail())
					{
						stx::logger::Error("ファイル読み込み失敗 failename:%s", filename);
					}
					std::istreambuf_iterator<uint8_t> it(ifs);
					std::istreambuf_iterator<uint8_t> last;
					raw_ptr data(new raw(it, last));

					if (!cache_enable) return data;

					//duplication check
					const auto fnv1hash = fnv_1_get_hash(data);
					if (resource_map_.find(fnv1hash) == resource_map_.end())
					{
						//not duplication
						resource_map_.insert(std::make_pair(fnv1hash, data));
					}
					path_map_.insert(std::make_pair(hash, fnv1hash));

					return data;
				}
			};

			raw_ptr nocache::operator [](const tstring& filename)
			{
				rfstream ifs(filename, std::ios::binary);
				if (ifs.fail())
				{
					stx::logger::Error("ファイル読み込み失敗 failename:%s", filename);
				}
				std::istreambuf_iterator<uint8_t> it(ifs);
				std::istreambuf_iterator<uint8_t> last;
				raw_ptr data(new raw(it, last));

				return data;
			}


		};
	};
};
