#include "javascript.h"
#include <stx/logger.hpp>

#pragma comment(lib,"icui18n.lib")
#pragma comment(lib,"icuuc.lib")
#pragma comment(lib, "v8_base_0.lib")
#pragma comment(lib, "v8_base_1.lib")
#pragma comment(lib, "v8_base_2.lib")
#pragma comment(lib, "v8_base_3.lib")
#pragma comment(lib, "v8_libbase.lib")
#pragma comment(lib, "v8_libplatform.lib")
#pragma comment(lib, "v8_nosnapshot.lib")

#include <boost/thread.hpp>
#include <boost/timer.hpp>

#include <deque>

//Classes
//#include "Sprite.h"

namespace js
{

	class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
	public:
		virtual void* Allocate(size_t length) {
			void* data = AllocateUninitialized(length);
			return data == NULL ? data : memset(data, 0, length);
		}
		virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
		virtual void Free(void* data, size_t) { free(data); }
	};


	using namespace v8;
	Javascript::Javascript() :
		assignglobalcount_(0u)
	{
	}


	Javascript::~Javascript()
	{
	}

	void Javascript::Initialize()
	{
		stx::logger::scope_message<stx::logger::begin_end> msg("V8 initialize.");

		V8::InitializeICU();
		Platform * platform = platform::CreateDefaultPlatform();
		V8::InitializePlatform(platform);
		V8::Initialize();

		ArrayBufferAllocator allocator;
		Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = &allocator;

		isolate_ = Isolate::New(create_params);

		//Locker locker(isolate_);
		{
			Isolate::Scope isolate_scope(isolate_);

			HandleScope handle(isolate_);

			/*
			 * memo
			 * Localはisolateに依存して解放を行うため、
			 * スコープを抜けてもisolateが生存している限り解放されない
			 */

			//Create olbal context
			Handle<ObjectTemplate> global_template = ObjectTemplate::New(isolate_);
			Handle<Context> context = Context::New(isolate_, nullptr, global_template);

			context_.Reset(isolate_, context);
			global_.Reset(isolate_, global_template);

		}

		/*
		 * スクリプト側からJavascriptオブジェクトにアクセスできるようにする
		 */

		{
			stx::logger::scope_message<stx::logger::begin_end> cmsg("Construct classes tree.");

			//registrar_.
			//	AddDefineInstanceTemplate<Sprite>("Sprite","");

			registrar_.Run();
		}

	}

	void Javascript::ResetContext()
	{
		Handle<ObjectTemplate> global_template = Local<ObjectTemplate>::New(isolate_,global_);
		Handle<Context> context = Context::New(isolate_, nullptr, global_template);
		context_.Reset(isolate_,context);
	}

	Registrar & Javascript::Regist()
	{
		/*
		* スクリプト側からJavascriptオブジェクトにアクセスできるようにする
		*/
		stx::logger::scope_message<stx::logger::begin_end> cmsg("Regist classes tree.");
		return registrar_;
	}

	void Javascript::RegistFlush()
	{
		stx::logger::scope_message<stx::logger::begin_end> cmsg("Construct classes tree.");
		registrar_.Run();
	}

	// とってもだるいので後回し
	// 参照先はv8.../src/d8-debug.cc
	void Javascript::HandleDebugEvent(const Debug::EventDetails & event_details)
	{
		// Handle on Debug Event

		// Get Current Isolate
		auto isolate = Isolate::GetCurrent();
		HandleScope scope(isolate);

		DebugEvent event = event_details.GetEvent();
		if (event != Break && event != Exception && event != AfterCompile) {
			return;
		}

		TryCatch try_catch(isolate);

		Local<String> to_json_fun_name = String::NewFromUtf8(isolate, "toJSONProtocol");
		Handle<Object> event_data = event_details.GetEventData();
		Local<Function> to_json_fun = Local<Function>::Cast(event_data->Get(to_json_fun_name));
		Local<Value> event_json = to_json_fun->Call(event_data, 0, NULL);

		if (try_catch.HasCaught())
		{
			String::Utf8Value message(try_catch.Message()->Get());
			stx::logger::Error(*message);
		}
	}

	void Javascript::ScriptAssign(const V8Block& block)
	{
		if (block)
		{
			Locker locker(this->isolate_);
			Isolate::Scope iso_scope(this->isolate_);
			HandleScope handle(this->isolate_);

			Local<Context> context = Local<Context>::New(this->isolate_,context_);
			Context::Scope ctx_scope(context);
			block(context);
		}
	}

	void Javascript::ScriptAssignTimeOut(const V8Block& block)
	{
		if (block)
		{
			//boost::thread
			Locker locker(this->isolate_);
			Isolate::Scope iso_scope(this->isolate_);
			HandleScope handle(this->isolate_);

			Local<Context> context = Local<Context>::New(this->isolate_, context_);
			Context::Scope ctx_scope(context);
			block(context);
		}
	}

	void Javascript::Execute(const tstring& script,
		const tstring& filename,
		const V8ValueCallBack& callback)
	{
		ScriptAssign(
			[&](const Handle<Context>& context)
		{
			auto source = String::NewFromUtf8(isolate_,script.c_str());

			v8::TryCatch trycatch;
			v8::Handle<v8::Value> result = Undefined(isolate_);

			std::string compile_path;
			compile_path = filename;

			auto compiled_script = Script::Compile(source, String::NewFromUtf8(isolate_,compile_path.c_str()));
			if (compiled_script.IsEmpty()) {

				// コンパイルエラー
				auto exception = trycatch.Exception();
				String::Utf8Value exception_str(exception);
				stx::logger::Error(unicode::ToTString(*exception_str));
				if (callback) {
					callback(Undefined(isolate_), *exception_str);
				}

			} else {
				result = compiled_script->Run();
				if (result.IsEmpty()) {

					// ランタイムエラー
					Handle<Value> exception;
					HandleScope scope(isolate_);

					if (trycatch.CanContinue()) {
						exception = trycatch.Exception();
					} else {
						exception = String::NewFromUtf8(isolate_,"Execution limit exceeded.");
					}

					String::Utf8Value exception_str(exception);
					stx::logger::Error(unicode::ToTString(*exception_str));
					if (callback) {
						callback(Undefined(isolate_), *exception_str);
					}

				} else {
					if (callback) {
						callback(result, "");
					}
				}
			}

		});

		boost::thread th = boost::thread([](){
			Isolate* iso = Isolate::New();
			iso->Enter();
			while (!iso->IdleNotification(500));
			iso->Exit();
			iso->Dispose();
		});
	}

	void Javascript::Load(const tstring& filename,
		const V8ValueCallBack& callback)
	{
		using namespace boost::filesystem;
		path script_path(filename);

		boost::timer t;
		if (exists(script_path))
		{
			std::ifstream ifs(script_path.string());
			tstring script(
				(std::istreambuf_iterator<char>(ifs)),
				std::istreambuf_iterator<char>());

			Execute(script, script_path.string(),
				[&callback](const Handle<Value>& result, const std::string& error)
			{
				if (callback) {
					callback(result, error);
				}
			});

		} else {
			tstring error = "Error: " + script_path.string() + " No such file.";
			if (callback) {
				callback(Undefined(isolate_), error);
			}
		}
	}

	void Javascript::ParseJSON(const tstring& json,
		const V8ValueCallBack& callback)
	{

	}

	uint32_t Javascript::NextAssignGlobalCount()
	{
		return assignglobalcount_++;
	}

	Isolate* Javascript::GetIsolate()
	{
		return isolate_;
	}

	Local<ObjectTemplate> Javascript::GetGlobal()
	{
		Local<ObjectTemplate> global = Local<ObjectTemplate>::New(isolate_, global_);
		return global;
	}

	tstring Javascript::GetInfo()
	{
		tstring info;
		info += tstring("V8 Javascript Engine ") + V8::GetVersion() + tstring("\n");

		//With(
		//	[&](const Handle<Context>& context)
		//{
		//	Handle<String> key = String::New("CoffeeScript");
		//	Handle<Object> compiler = context->Global()->GetHiddenValue(key)->ToObject();
		//	if (compiler->Has(String::New("VERSION"))) {
		//		info += "CoffeeScript ";
		//		info += *String::AsciiValue(compiler->Get(String::New("VERSION"))->ToString());
		//		info += "\n";
		//	}
		//});

		//info += "Sugar Library v1.2.5";

		return info;
	}


	JavascriptPtr Javascript::GetInstance()
	{
		static JavascriptPtr instance(new Javascript());
		return instance;
	};


	void Javascript::SetAllowEval(bool allow)
	{
		Local<Context> context = Local<Context>::New(this->isolate_, context_);
		context->AllowCodeGenerationFromStrings(allow);
	}

	Local<String> Javascript::JSString(tstring string)
	{
		return String::NewFromUtf8(GetInstance()->GetIsolate(), string.c_str());
	}

	Registrar::Registrar()
	{
		definitionmap_ = std::make_shared<DefinitionMap>();
	}

	Registrar::~Registrar()
	{

	}

	void Registrar::Run()
	{
		//Get Pointer of List
		const auto mapptr = definitionmap_;

		Javascript::GetInstance()->ScriptAssign(
		[&](const Handle<Context>& context)
		{
			auto js = Javascript::GetInstance();

			if (mapptr)
			{


				//グローバルレベルへクラス登録 
				for (auto &definition : mapptr->Cast())
				{
					auto func = definition.second;
					auto space = definition.first;
					if (!space.empty())
					{
						std::deque<tstring> pusher;
						boost::split(pusher, space, boost::is_any_of("."));

						tstring last_keystr = pusher.front();
						pusher.pop_front();

						Local<ObjectTemplate> setter = ObjectTemplate::New(js->GetIsolate());

						//最上位キーをグローバルへ紐づける
						Local<String> key = String::NewFromUtf8(js->GetIsolate(), last_keystr.c_str());
						js->GetGlobal()->Set(key, setter);
						
						for (auto keystr : pusher)
						{
							Local<String> key = String::NewFromUtf8(js->GetIsolate(), keystr.c_str());
							Local<ObjectTemplate> new_setter = ObjectTemplate::New(js->GetIsolate());
							setter->Set(key, new_setter);
							setter = new_setter;
						}
						func(&setter);
					} else{
						func(&js->GetGlobal());
					}
				}

				js->ResetContext();


				//クラスを伴わない名前空間付き関数はまた別の方法で登録する

				//auto global_template = 
				//auto script_object = global_template->NewInstance();
				//context->Global()->Set(js->NextAssignGlobalCount(), script_object);
			}
		});
	}

};