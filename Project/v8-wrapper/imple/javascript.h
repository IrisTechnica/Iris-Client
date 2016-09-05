#pragma once
#include "../javascript.h"

//implemente of template function
namespace js
{

	template <class _Ty>
	void Registrar::SetObjectTemplate(const tstring& classname, Handle<ObjectTemplate>* object)
	{
		auto iso = Javascript::GetInstance()->GetIsolate();
		auto func = FunctionTemplate::New(iso,Construct<_Ty>);

		tstring utf8_classname = unicode::sjis2utf8(classname);
		Handle<String> v8_classname = String::NewFromUtf8(iso, utf8_classname.c_str());
		func->SetClassName(v8_classname);

		auto instance_template = func->InstanceTemplate();
		instance_template->SetInternalFieldCount(1);

		_Ty::DefineInstanceTemplate(&instance_template);

		(*object)->Set(v8_classname, func, PropertyAttribute(ReadOnly));

	}

	template <class _Fn>
	void Registrar::SetFunction(Handle<ObjectTemplate>* object, const tstring& name, _Fn func)
	{
		auto iso = Javascript::GetInstance()->GetIsolate();

		auto &instance_template = *object;
		instance_template->Set(String::NewFromUtf8(iso, name.c_str()), FunctionTemplate::New(iso,func));
	}

	template<class _Gt, class _St>
	void Registrar::SetProperty(Handle<ObjectTemplate>* object, const tstring& name, _Gt getter, _St setter)
	{
		auto iso = Javascript::GetInstance()->GetIsolate();

		auto& instance_template = *object;
		instance_template->SetAccessor(String::NewFromUtf8(iso, name.c_str()), getter, setter);
	}

	template<class _Cs>
	void Registrar::SetConstant(Handle<ObjectTemplate>* object, const tstring& name, _Cs value)
	{
		auto iso = Javascript::GetInstance()->GetIsolate();

		auto& instance_template = *object;
		instance_template->Set(String::NewFromUtf8(iso, name.c_str()), value);
	}

	template<class _Ty>
	void Registrar::Construct(const FunctionCallbackInfo<Value>& args)
	{
		auto iso = Javascript::GetInstance()->GetIsolate();

		_Ty* instance = new _Ty();
		stx::logger::Debug("[JS] class construct : %s", typeid(instance).name());
		Local<Object> this_obj = args.This();
		this_obj->SetInternalField(0, External::New(iso, instance));
		Persistent<Object> holder;
		holder.Reset(iso, this_obj);
		holder.SetWeak<Value,_Ty>(instance, Destruct<_Ty>);
	}
	template<class _Ty>
	void Registrar::Destruct(const WeakCallbackData<Value, _Ty>& data)
	{
		auto param = data.GetParameter();
		stx::logger::Debug("[JS] class destruct : %s", typeid(param).name());
		if(param)delete param;
	}

	template<class _Ty>
	Registrar::this_type& Registrar::AddDefineInstanceTemplate(const tstring& classname, const tstring name_space)
	{
		auto detect_func = [classname](Handle<ObjectTemplate>* object)
		{
			SetObjectTemplate<_Ty>(classname, object);
		};

		definitionmap_->add_func(detect_func,name_space);

		return *this;
	}

};