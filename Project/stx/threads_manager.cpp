#include "threads_manager.hpp"

std::unordered_map<stx::tstring_symbol, ThreadsPtr> ThreadsManager::threads_map;

ThreadsPtr ThreadsManager::threads(stx::tstring_symbol name)
{
	if (threads_map.find(name) != threads_map.end())return threads_map[name];
	auto &ret = threads_map[name] = ThreadsPtr(new Threads());
	return ret;
}
