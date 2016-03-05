#pragma once
#include "threads.hpp"
#include "tstring_symbol.hpp"

class ThreadsManager
{
private:
	static std::unordered_map<stx::tstring_symbol, ThreadsPtr> threads_map;

public:

	static ThreadsPtr threads(stx::tstring_symbol name);
};