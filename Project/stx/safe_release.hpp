#pragma once
#include <type_traits>


#if !defined(safe_release)
#define safe_release(p) { if(p){(p)->Release();(p) = nullptr;} }
#endif

class safe_releaser
{
public:
	template<class Any, typename std::enable_if<std::is_pointer<Any>::value>::type *& = stx::enabler>
	safe_releaser& operator <<(Any r)
	{
		safe_release(r);
		return *this;
	}
};

#ifdef STX_SAFE_RELEASE

extern safe_releaser chain_release;

#else

safe_releaser chain_release;

#define STX_SAFE_RELEASE
#endif