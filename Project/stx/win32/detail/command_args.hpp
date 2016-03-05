#pragma once

#include <Windows.h>
#include <boost/program_options.hpp>

#include "../singleton.hpp"
#include "../tstring_symbol.hpp"

namespace stx
{

class CommandArgs
	: public singleton<CommandArgs>
{
public:
	CommandArgs()
	{
	}

private:
	boost::program_options::variables_map variables_map_;
};

};