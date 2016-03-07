#pragma once

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "tstring_symbol.hpp"
#include "cast.h"
#include "singleton.hpp"
#include "filesystem/filesystem.h"
#include "typedef.h"

namespace stx
{

using boost::filesystem::path;
using tptree = boost::property_tree::tptree;
class config_file :
	 public refered_singleton<config_file>
{
public:
	class refered_config_file;

private:
	tptree	tree_;
	path	relpath_;

public:
	config_file()
	{
	}


	void set_directory(path directory)
	{

		using namespace boost::filesystem;

		if (!exists(directory))
			create_directories(directory);

		if (relpath_.has_filename())
			relpath_ = directory / relpath_.filename();
		else
			relpath_ = directory;
	}

	void load(path filename)
	{
		if (relpath_.has_filename())
			relpath_ = relpath_.remove_filename() / filename;
		else
			relpath_ /= filename;

		using namespace boost::filesystem;
		using namespace boost::property_tree;

		if (exists(relpath_))
			read_json(relpath_.string(), tree_);
		else
			tree_ = tptree();
	}

	refered_config_file operator [](tstring path)
	{
		return refered_config_file(this,&tree_,path);
	}


	const tptree& get_child(tstring path)
	{
		return tree_.get_child(path);
	}

	void save()
	{
		using namespace boost::property_tree;

		auto old_pwd = stx::filesystem::current_path_adjustment();

		auto actual = boost::filesystem::absolute(relpath_);
		write_json(actual.string(), tree_);

		stx::filesystem::set_current_directory(old_pwd);
	}

	class refered_config_file
	{
	private:
		typedef refered_config_file this_type;

		config_file* refered_config_;
		tptree* refered_tree_;
		const tstring path_;

	public:
		refered_config_file(config_file* config,tptree* tree,const tstring path) :
			refered_config_(config),
			refered_tree_(tree),
			path_(path)
		{
		}

		template<class _Ty>
		_Ty get_default(_Ty default_value)
		{
			boost::optional<tstring> optional = refered_tree_->get_optional<tstring>(path_);
			if (optional == boost::none)
			{
				//set default
				refered_tree_->put(path_, flexible_cast<tstring>(default_value));
				refered_config_->save();

				return default_value;
			}
			return flexible_cast<_Ty>(optional.get());
		}

		template<class _Ty>
		_Ty get()
		{
			return refered_tree_->get<_Ty>(path_);
		}

		template<class _Vt>
		this_type& operator << (const _Vt& value)
		{
			refered_tree_->put(path_, flexible_cast<tstring>(value));
			refered_config_->save();
			return *this;
		}

		template<>
		this_type& operator << <tptree>(const tptree& tree)
		{
			refered_tree_->put_child(path_, tree);
			refered_config_->save();
			return *this;
		}
		
	};
};

};