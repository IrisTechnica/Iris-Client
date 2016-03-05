#pragma once
#include <thread>
#include <unordered_map>
#include <functional>
#include <atomic>

#include "unicode.hpp"
#include "singleton.hpp"

class Threads
{
private:
	std::unordered_map<tstring,std::thread> workers;
	std::unordered_map<tstring, std::function<void()>> idle_workers;

	std::atomic<bool> join_wait_flag;
public:
	Threads();
	~Threads();

	void put_idles(std::unordered_map<tstring, std::function<void()>>& idles);
	void start();
	void joins();

	// operators
	// no thread safe
	std::thread& operator[](tstring name);
	std::unordered_map<tstring, std::thread>& get();

	// thread safe
	bool is_join_wait();	// is Threads waiting all workers?
	
};

typedef std::shared_ptr<Threads> ThreadsPtr;