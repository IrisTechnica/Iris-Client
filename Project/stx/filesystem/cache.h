#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <type_traits>

#include "../tstring_symbol.hpp"
#include "../cast.h"

namespace stx
{
	namespace filesystem
	{
		typedef std::basic_string<uint8_t, std::char_traits<uint8_t>, std::allocator<uint8_t> > raw;
		typedef std::basic_ifstream<uint8_t, std::char_traits<uint8_t> > rfstream;

		typedef std::shared_ptr<raw> raw_ptr;
		
		typedef struct
		{
			size_t		size;
			uint8_t*	data;
		} pod_rawfile;

		namespace entity
		{
			class cache
			{
			private:
				std::unordered_map<stx::tstring_symbol, uint64_t> path_map_;
				std::unordered_map<uint64_t, raw_ptr> resource_map_;
			public:
				cache();
				~cache();

				raw_ptr operator [](const tstring& filename);

			private:
				uint64_t fnv_1_get_hash(const raw_ptr& data);

			};

			class nocache
			{
			public:
				raw_ptr operator [](const tstring& filename);
			};

		};

		//Grobal Cache Data
		static entity::cache cache;
		static entity::nocache nocache;
	};
};

//Regist Cast Method
template <>
struct flexible_cast_entity < stx::filesystem::pod_rawfile, stx::filesystem::raw >
{
	static stx::filesystem::pod_rawfile do_cast(stx::filesystem::raw &src)
	{
		using namespace stx::filesystem;
		pod_rawfile ret;
		ret.size = src.size();
		ret.data = &(*src.begin());
		return ret;
	}
};

template <>
struct flexible_cast_entity < tstring, stx::filesystem::raw >
{
	static tstring do_cast(stx::filesystem::raw &src)
	{
		using namespace stx::filesystem;
		tstring ret;
		for (auto dat : src)
		{
			ret += static_cast<char>(dat);
		}
		return ret;
	}
};
