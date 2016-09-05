#pragma once

#include <stx/singleton.hpp>
#include <stx/typetraits/typetraits.hpp>

#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/back/state_machine.hpp>

namespace state
{
	namespace front = boost::msm::front;

	namespace states
	{
		// States
		struct idle : front::state<> {};
		struct start : front::state<> {};

	}

	namespace events
	{
		// Events
		struct start {};

	}

	namespace detail
	{
		using namespace front;
		struct MachineDefinition : front::state_machine_def<MachineDefinition>
		{
			struct transition_table : boost::mpl::vector
				//			source			event			target
				< _row< states::idle,	events::start,	states::start>
				> {};

			typedef states::idle inital_state;
		};
	}


	class StateMachine : stx::refered_singleton<StateMachine>
	{
	public:
		typedef boost::msm::back::state_machine<detail::MachineDefinition> machine_type;

		void start()
		{
			machine.start();
		}

		template<class State, typename std::enable_if<std::is_base_of<front::state<>>::value>::type *& = stx::enabler>
		void operator=(const State& right)
		{
			machine.process_event(right);
		}

	private:
		machine_type machine;
	};

}