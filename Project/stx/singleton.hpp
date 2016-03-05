#pragma once

#include "config.hpp"

#include <memory>
#include <boost/noncopyable.hpp>

namespace stx
{

template<class _Sgl>
class singleton :
	public boost::noncopyable
{
	typedef std::shared_ptr<_Sgl> _SglPtr;
public:
	static _SglPtr GetInstance();
};

template<class _Sgl>
typename singleton<_Sgl>::_SglPtr singleton<_Sgl>::GetInstance()
{
	static _SglPtr instance(new _Sgl());
	return instance;
}

template<class _Sgl>
class thread_local_singleton :
	public boost::noncopyable
{
	typedef std::shared_ptr<_Sgl> _SglPtr;
public:
	static _SglPtr GetInstance();

};

template<class _Sgl>
typename thread_local_singleton<_Sgl>::_SglPtr thread_local_singleton<_Sgl>::GetInstance()
{
	thread_local static _SglPtr instance(new _Sgl());

	return instance;
}

template<class _Sgl>
class refered_singleton :
	public boost::noncopyable
{
	typedef std::shared_ptr<_Sgl> _SglPtr;
public:
	static typename _SglPtr::element_type& GetRefInstance();
	static _SglPtr GetInstance();
};

template<class _Sgl>
inline typename refered_singleton<_Sgl>::_SglPtr::element_type& refered_singleton<_Sgl>::GetRefInstance()
{
	return *GetInstance().get();
}

template<class _Sgl>
typename refered_singleton<_Sgl>::_SglPtr refered_singleton<_Sgl>::GetInstance()
{
	static _SglPtr instance(new _Sgl());
	return instance;
}


//template<class _Sgl>
//inline vector<typename thread_local_singleton<_Sgl>::_SglPtr> thread_local_singleton<_Sgl>::GetAllThreadObjects()
//{
//	return ReferenceThreadObjectList();
//}
//
//template<class _Sgl>
//inline vector<typename thread_local_singleton<_Sgl>::_SglPtr>& thread_local_singleton<_Sgl>::ReferenceThreadObjectList()
//{
//	// TODO: return ステートメントをここに挿入します
//}

//template<class _Sgl>
//typename std::vector<typename thread_local_singleton<_Sgl>::_SglPtr>& thread_local_singleton<_Sgl>::ReferenceThreadObjectList()
//{
//	static std::vector<_SglPtr> object_list;
//	return object_list;
//}


//#if defined(STX_USE_ATOMICSINGLETON)

};

#include <mutex>

namespace stx
{

template<class _Sgl>
class atomic_singleton :
	public boost::noncopyable
{
	typedef std::shared_ptr<_Sgl> _SglPtr;

private:
	std::mutex	mutex_;

public:
	static _SglPtr GetInstance();
};

template<class _Sgl>
typename atomic_singleton<_Sgl>::_SglPtr atomic_singleton<_Sgl>::GetInstance()
{
	std::mutex::scoped_lock lock(mutex_);
	static _SglPtr instance(new _Sgl());
	return instance;
}


//#endif


};