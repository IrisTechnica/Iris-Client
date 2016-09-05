#pragma once
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

#include <v8/v8.h>
#include <v8/v8-debug.h>
#include <v8/libplatform/libplatform.h>
#include <boost/timer.hpp>
#include <boost/noncopyable.hpp>

#include <stx/tstring_symbol.hpp>

namespace js
{
	using namespace v8;
	//namespace ptree = boost::property_tree;

	class DefinitionMap;

	typedef std::function<void(Handle<ObjectTemplate>*)>		DefineInstanceTemplateFunction;
	typedef std::pair<tstring, DefineInstanceTemplateFunction>	DefinitionPair;
	typedef std::vector<DefinitionPair>							DefinitionList;
	typedef std::shared_ptr<DefinitionMap>						DefinitionMapPtr;

	class DefinitionMap
	{
	private:
		DefinitionList list_;

	public:

		inline bool empty()
		{
			return list_.empty();
		}

		inline void add_func(const DefineInstanceTemplateFunction func,tstring name_space = _T(""))
		{
			list_.push_back(std::make_pair(name_space,func));
		}


		const DefinitionList& Cast() const
		{
			return list_;
		}

	};

	class Registrar
	{
	private:
		typedef Registrar this_type;
	public:

		Registrar();
		~Registrar();

		//Add class to V8
		template<class _Ty>
		static void SetObjectTemplate(const tstring& classname, Handle<ObjectTemplate>* object);

		template<class _Fn>
		static void SetFunction(Handle<ObjectTemplate>* object, const tstring& name, _Fn func);

		template<class _Gt, class _St>
		static void SetProperty(Handle<ObjectTemplate>* object, const tstring& name, _Gt getter, _St setter);

		template<class _Cs>
		static void SetConstant(Handle<ObjectTemplate>* object, const tstring& name, _Cs value);

		template<class _Ty>
		static void Construct(const FunctionCallbackInfo<Value>& args);

		template<class _Ty>
		static void Destruct(const WeakCallbackData<Value, _Ty>& data);

		template<class _Ty>
		this_type& AddDefineInstanceTemplate(const tstring& classname,const tstring name_space = "");

		void Run();
	private:
		DefinitionMapPtr definitionmap_;
	};

	class JSBaseObject
	{
	public:
		static void DefineInstanceTemplate(Handle<ObjectTemplate>* object);
	};

	struct TimerEvent {
		v8::Persistent<v8::Function> function;
		std::vector<v8::Persistent<v8::Value>> args;
		int delay;
		bool interval;
		boost::timer timer;
	};
	typedef std::shared_ptr<TimerEvent> TimerEventPtr;


	class Javascript;
	typedef std::shared_ptr<Javascript> JavascriptPtr;

	typedef std::function<void(const Handle<Value>&, const std::string)>	V8ValueCallBack;
	typedef std::function<void(const Handle<Context>&)>						V8Block;
	typedef std::shared_ptr<std::function<void(const Handle<Context>&)>>	V8BlockPtr;

	//Singleton without manager
	class Javascript :
		public boost::noncopyable
	{
	public:
		~Javascript();

		void Initialize();
		void ScriptAssign(const V8Block& block);

		//This Function is Deprecated
		void ScriptAssignTimeOut(const V8Block& block);

		void Execute(const tstring& script,
			const tstring& filename = _T(""),
			const V8ValueCallBack& callback = V8ValueCallBack());

		void Load(const tstring& filename,
			const V8ValueCallBack& callback = V8ValueCallBack());

		void ParseJSON(const tstring& json,
			const V8ValueCallBack& callback);

		void SetAllowEval(bool);
		bool GetAllowEval();

		uint32_t NextAssignGlobalCount();

		Isolate* GetIsolate();
		tstring GetInfo();
		Local<ObjectTemplate> GetGlobal();
		void ResetContext();

		Registrar& Regist();
		void RegistFlush();

		static void HandleDebugEvent(const Debug::EventDetails& event_details);

		//Static Function
		static JavascriptPtr GetInstance();
		static void SetMaxExecutionTime(unsigned int second);
		static Local<String> JSString(tstring);

	private:
		Javascript();
		void Terminate();
		void SetBuiltins();
		void UpdateTimerEvents();

	private:

		uint32_t assignglobalcount_;
		Isolate *isolate_;

		Persistent<Context> context_;
		Persistent<ObjectTemplate>	global_;

		Registrar registrar_;

	};

};

#include "imple/javascript.h"