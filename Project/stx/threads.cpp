#include "Threads.hpp"
#include "logger.hpp"


Threads::Threads() :
	join_wait_flag(false)
{
}


Threads::~Threads()
{
}

void Threads::put_idles(std::unordered_map<tstring, std::function<void()>>& idles)
{
	for (auto& idle : idles)
	{
		auto key = idle.first;
		auto func = idle.second;

		if (idle_workers.find(key) != idle_workers.end())
		{
			// idle_workers is not has key
			idle_workers[key] = func;
		}
		else {
			// !!warning already idle_workers has key
			stx::logger::Warning("[Therads] %1% already has key. func is swaped.", key);
			idle_workers[key] = func;
		}
	}
}

void Threads::start()
{
	for (auto& idle : idle_workers)
	{
		auto key = idle.first;
		auto func = idle.second;

		workers[key] = std::thread(func);
	}
	idle_workers.clear();
}

void Threads::joins()
{
	join_wait_flag = true;
	for (auto& worker : workers)
	{
		auto& thread = worker.second;
		thread.join();
	}
	join_wait_flag = false;
}

std::thread & Threads::operator[](tstring name)
{
	return workers.at(name);
}

std::unordered_map<tstring, std::thread>& Threads::get()
{
	return workers;
}

bool Threads::is_join_wait()
{
	return join_wait_flag;
}
