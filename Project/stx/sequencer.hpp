#pragma once
#include <functional>
#include <unordered_map>
#include "unicode.hpp"
#include "tstring_symbol.hpp"
#include <boost/logic/tribool.hpp>
#include <chrono>
#include <boost/detail/select_type.hpp>
#include "typetraits/switch_types.hpp"
#include <thread>
#include <boost/optional.hpp>
#include <boost/coroutine/all.hpp>
#include "range.hpp"

namespace stx
{
	enum SequencerMode
	{
		frames			= 0x00, // frames mode
		times			= 0x10, // times mode
		microseconds	= 0x11, // times detail mode
		milliseconds,
		seconds,
		minutes,
		hours,
	};

	// Threading Class
	template<SequencerMode _Mode>
	class Sequencer
	{
	public:
		typedef Sequencer<_Mode> this_type;

		typedef typename stx::detail::switch_types<SequencerMode>::
			which<
				frames,
				times,
				microseconds,
				milliseconds,
				seconds,
				minutes,
				hours
			>::select<_Mode>::template
			types<
				uint32_t,
				std::chrono::milliseconds, // times mode default
				std::chrono::microseconds,
				std::chrono::milliseconds,
				std::chrono::seconds,
				std::chrono::minutes,
				std::chrono::hours
			>::type data_type;

		static bool const is_timer_mode = std::integral_constant<bool, (_Mode & times)>::value;

		typedef boost::optional<std::thread> TimerThreadingFunction;
		typedef std::function<void(boost::coroutines::coroutine<bool>::push_type&)> FrameThreadingFunction;

		typedef std::integral_constant<bool, true>	sequence_end;
		typedef std::integral_constant<bool, false>	sequence_running;

		typedef typename boost::detail::if_true<is_timer_mode>::template then<TimerThreadingFunction, FrameThreadingFunction>::type ThreadingFunction;
		typedef typename boost::detail::if_true<is_timer_mode>::template then<void*, boost::coroutines::coroutine<bool>::push_type >::type FrameOnlyPushType; // only used on frame mode
		typedef typename boost::detail::if_true<is_timer_mode>::template then<void*, boost::coroutines::coroutine<bool>::pull_type >::type FrameOnlyPullType; // only used on frame mode

		struct SequencerHandle
		{
			uint32_t count;				// 当該関数が呼ばれた回数
			tstring	handlename;			// ハンドルネーム、イベント以外は空
			std::exception exception;	// エラーが存在する場合、値が格納される
			SequencerHandle() :
				count(0),
				handlename(""),
				exception()
			{}
		};

		typedef std::function<void(SequencerHandle& e)> FunctionCallback;

		struct FunctionEvent
		{
			FunctionCallback function;
			uint32_t call_count			= 1; // 呼び出す回数
			data_type delay_time		= 0; // 遅延実行の指定
			data_type call_interval		= 0; // 呼び出し間隔
		};

	private:
		std::vector<FunctionEvent> trigger_function_list;
		std::unordered_map<tstring_symbol,FunctionEvent> event_function_list;
		ThreadingFunction sequencer_function;
		FrameOnlyPullType co;

		void trigger_regist(FunctionEvent function_event);
		void event_regist(tstring event_name, FunctionEvent function_event);

	public:
		explicit Sequencer(
			const FunctionCallback& function,
			uint32_t call_count = 1,
			data_type delay_time = 0,
			data_type call_interval = 0
			);
		~Sequencer();

		this_type& then(
			const FunctionCallback& function,
			uint32_t call_count = 1,
			data_type delay_time = 0,
			data_type call_interval = 0
			);

		this_type& oncomplete(const FunctionCallback& function);
		this_type& onfaild(const FunctionCallback& function);

		void run();
		void operator()();

	private:

		// SFINAE
		template<bool check>
		struct internal_process
		{
			// Timer
			static void run(this_type& refered)
			{
				refered.sequencer_function = thread([&]() {
					try {
						if (!refered.trigger_function_list.empty())
						{
							SequencerHandle handle;
							auto index = 0u;
							while (index < refered.trigger_function_list.size())
							{
								auto &stack = refered.trigger_function_list[index];
								this_thread::sleep_for(stack.delay_time);
								for (auto call_count : stx::range(stack.call_count + 1))
								{
									handle.count = call_count;
									stack.function(handle);
									this_thread::sleep_for(stack.call_interval);
								}
								index++;
							}
						}
					}
					catch (std::exception& e) {
						auto name = tstring("onfaild");
						auto failed_hash = tstring_symbol(name);
						if (refered.event_function_list.find(failed_hash) != refered.event_function_list.end())
						{
							SequencerHandle handle;
							handle.handlename = name;
							refered.event_function_list.at(failed_hash).function(handle);
						}
					}
					// finally
					{
						auto name = tstring("oncomplete");
						auto complete_hash = tstring_symbol(name);
						if (refered.event_function_list.find(complete_hash) != refered.event_function_list.end())
						{
							SequencerHandle handle;
							handle.handlename = name;
							refered.event_function_list.at(complete_hash).function(handle);
						}
					}
				});
			};
			static void step(this_type& refered) {};
			static void exit(this_type& refered) {
				refered.sequencer_function->join();
			};
		};

		template<>
		struct internal_process<false>
		{
			// Frame
			static void run(this_type& refered)
			{

				refered.sequencer_function = [&](FrameOnlyPushType &boost_yield) {
					try {
						if (!refered.trigger_function_list.empty())
						{
							SequencerHandle handle;
							auto index = 0u;
							while (index < refered.trigger_function_list.size())
							{
								auto &stack = refered.trigger_function_list[index];
								auto delay_count = stack.delay_time;
								// delay count is more 0 then return yield
								if (delay_count > 0)
								{
									delay_count--;
									boost_yield(sequence_running::value);
								}
								for (auto call_count : stx::range(stack.call_count + 1))
								{
									handle.count = call_count;
									stack.function(handle);
									boost_yield(sequence_running::value);
									auto call_interval = stack.call_interval;
									if (call_interval > 0)
									{
										call_interval--;
										boost_yield(sequence_running::value);
									}
								}
								index++;
							}
						}
					}
					catch (std::exception& e) {
						auto name = tstring("onfaild");
						auto failed_hash = tstring_symbol(name);
						if (refered.event_function_list.find(failed_hash) != refered.event_function_list.end())
						{
							SequencerHandle handle;
							handle.handlename = name;
							refered.event_function_list.at(failed_hash).function(handle);
						}
					}
					// finally
					{
						auto name = tstring("oncomplete");
						auto complete_hash = tstring_symbol(name);
						if (refered.event_function_list.find(complete_hash) != refered.event_function_list.end())
						{
							SequencerHandle handle;
							handle.handlename = name;
							refered.event_function_list.at(complete_hash).function(handle);
						}
					}
				};
				
				refered.co.swap(FrameOnlyPullType(refered.sequencer_function));
			};
			static void step(this_type& refered)
			{
				if(refered.co)refered.co();
			};
			static void exit(this_type& refered) {
			};
		};

	};
	 
	template<SequencerMode _Mode>
	inline void Sequencer<_Mode>::trigger_regist(FunctionEvent function_event)
	{
		trigger_function_list.push_back(function_event);
	}

	template<SequencerMode _Mode>
	inline void Sequencer<_Mode>::event_regist(tstring event_name, FunctionEvent function_event)
	{
		auto event_hash = tstring_symbol(event_name);
		event_function_list[event_hash] = function_event;
	}

	template<SequencerMode _Mode>
	inline Sequencer<_Mode>::Sequencer(const FunctionCallback & function, uint32_t call_count, data_type delay_time, data_type call_interval)
	{
		trigger_function_list.clear();
		event_function_list.clear();

		FunctionEvent function_event;

		function_event.function = function;
		function_event.call_count = call_count;
		function_event.delay_time = delay_time;
		function_event.call_interval = call_interval;

		trigger_regist(function_event);
	}

	template<SequencerMode _Mode>
	inline Sequencer<_Mode>::~Sequencer()
	{
		internal_process<is_timer_mode>::exit(*this);
	}

	template<SequencerMode _Mode>
	inline typename Sequencer<_Mode>::this_type & Sequencer<_Mode>::then(const FunctionCallback & function, uint32_t call_count, data_type delay_time, data_type call_interval)
	{
		FunctionEvent function_event;

		function_event.function = function;
		function_event.call_count = call_count;
		function_event.delay_time = delay_time;
		function_event.call_interval = call_interval;

		trigger_regist(function_event);

		return *this;
	}

	template<SequencerMode _Mode>
	inline typename Sequencer<_Mode>::this_type & Sequencer<_Mode>::oncomplete(const FunctionCallback & function)
	{
		FunctionEvent function_event;

		function_event.function = function;

		event_regist("oncomplete", function_event);

		return *this;
	}

	template<SequencerMode _Mode>
	inline typename Sequencer<_Mode>::this_type & Sequencer<_Mode>::onfaild(const FunctionCallback & function)
	{
		FunctionEvent function_event;

		function_event.function = function;

		event_regist("onfaild", function_event);

		return *this;
	}

	template<SequencerMode _Mode>
	inline void Sequencer<_Mode>::run()
	{
		internal_process<is_timer_mode>::run(*this);
	}

	template<SequencerMode _Mode>
	inline void Sequencer<_Mode>::operator()()
	{
		internal_process<is_timer_mode>::step(*this);
	}

	//template<SequencerMode _Mode>
	//inline void Sequencer<_Mode, typename std::enable_if<Sequencer<_Mode>::is_timer_mode>::type *& = stx::enabler>::run()
	//{
	//	// Timer
	//	sequencer_function = thread([&]() {
	//		try {
	//			if (!trigger_function_list.empty())
	//			{
	//				SequencerHandle handle;
	//				auto index = 0u;
	//				while (index < trigger_function_list.size())
	//				{
	//					auto &stack = trigger_function_list[index];
	//					this_thread::sleep_for(stack.delay_time);
	//					for (auto call_count : stx::range(stack.call_count + 1))
	//					{
	//						handle.count = call_count;
	//						stack.function(handle);
	//						this_thread::sleep(stack.call_interval);
	//					}
	//					index++;
	//				}
	//			}
	//			catch (std::exception& e) {
	//				auto name = tstring("onfaild");
	//				auto failed_hash = tstring_symbol(name);
	//				if (event_function_list.find(failed_hash) != event_function_list.end())
	//				{
	//					SequencerHandle handle;
	//					handle.name = name;
	//					event_function_list.at(faild_hash)(handle);
	//				}
	//			}
	//			// finally
	//			{
	//				auto name = tstring("oncomplete");
	//				auto complete_hash = tstring_symbol(name);
	//				if (event_function_list.find(complete_hash) != event_function_list.end())
	//				{
	//					SequencerHandle handle;
	//					handle.name = name;
	//					event_function_list.at(complete_hash)(handle);
	//				}
	//			}
	//		}
	//	});
	//}

	//template<SequencerMode _Mode>
	//inline void Sequencer<_Mode>::run(typename std::enable_if<!Sequencer<_Mode>::is_timer_mode>::type *& = stx::enabler)
	//{
	//	sequencer_function = [&](FrameOnlyPushType &boost_yield) {
	//		try {
	//			if (!trigger_function_list.empty())
	//			{
	//				SequencerHandle handle;
	//				auto index = 0;
	//				while (index < trigger_function_list.size())
	//				{
	//					auto &stack = trigger_function_list[index];
	//					auto delay_count = stack.delay_count;
	//					// delay count is more 0 then return yield
	//					if (delay_count > 0)
	//					{
	//						delay_count--;
	//						boost_yield(sequence_running::value);
	//					}
	//					for (auto call_count : stx::range(stack.call_count + 1))
	//					{
	//						handle.count = call_count;
	//						stack.function(handle);
	//						boost_yield(sequence_running::value);
	//						auto call_interval = stack.call_interval;
	//						if (call_interval > 0)
	//						{
	//							call_interval--;
	//							boost_yield(sequence_running::value);
	//						}
	//					}
	//					index++;
	//				}
	//			}
	//		}
	//		catch (std::exception& e) {
	//			auto name = tstring("onfaild");
	//			auto failed_hash = tstring_symbol(name);
	//			if (event_function_list.find(failed_hash) != event_function_list.end())
	//			{
	//				SequencerHandle handle;
	//				handle.name = name;
	//				event_function_list.at(faild_hash)(handle);
	//			}
	//		}
	//		// finally
	//		{
	//			auto name = tstring("oncomplete");
	//			auto complete_hash = tstring_symbol(name);
	//			if (event_function_list.find(complete_hash) != event_function_list.end())
	//			{
	//				SequencerHandle handle;
	//				handle.name = name;
	//				event_function_list.at(complete_hash)(handle);
	//			}
	//		}
	//	};

	//	co(sequence_function);
	//}

	//template<SequencerMode _Mode>
	//inline void Sequencer<_Mode>::operator()(typename std::enable_if<!Sequencer<_Mode>::is_timer_mode>::type *& = stx::enabler)
	//{
	//	co();
	//}

};