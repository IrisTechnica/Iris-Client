#pragma once
// get IUnkwon class of base class in all com object
//#include <Unknwn.h>
// have to include the singleton
#include "singleton.hpp"

namespace stx
{
	// fwd declaration
	template <class _Ty>
	class com_ptr;


	namespace internal
	{
		template<class _Ty>
		class com_ptr_manager : 
			public singleton<com_ptr_manager<_Ty>>
		{
		public:
			typedef com_ptr<_Ty> value_type;
		private:
			std::vector<value_type> com_list;
		public:
			void Add(value_type ptr)
			{
				com_list.push_back(ptr);
				std::sort(com_list.begin(), com_list.end(),
					[](const value_type& t, const value_type& u)->const bool
				{
					return t.Get() < u.Get();
				});
				com_list.erase(std::unique(com_list.begin(), com_list.end()),com_list.end());
			}

			~com_ptr_manager()
			{
				for (auto item : com_list)
				{
					if(item.Get())while(item->Release() > 0);
				}
			}
		};
	};
	template <class _Ty>
	class com_ptr
	{
	private:
		_Ty*		interface_;
		unsigned	refcount_;

	public:
		com_ptr() :
			interface_(nullptr),
			refcount_(0)
		{
		}

		explicit com_ptr(_Ty* _interface)
		{
			if (_interface)
				refcount_ = _interface->AddRef();
			interface_ = _interface;
		}

		explicit com_ptr(std::nullptr_t null)
		{
			if (interface_)
				refcount_ = interface_->Release();
			if (refcount_ == 0)
				interface_ = nullptr;
		}

		com_ptr(const com_ptr& right)
		{
			if (right.interface_)
				refcount_ = right.interface_->AddRef();
			interface_ = right.interface_;
		}

		~com_ptr()
		{
			if (interface_)
				refcount_ = interface_->Release();
			if (refcount_ == 0)
				interface_ = nullptr;
		}


		void operator=(_Ty* _interface)
		{
			if (interface_)
				refcount_ = interface_->Release();
			interface_ = _interface;
			internal::template com_ptr_manager<_Ty>::GetInstance()->Add(*this);
		}

		bool operator==(long addr)
		{
			return addr == (long)interface_;
		}

		bool operator!=(long addr)
		{
			return addr != (long)interface_;
		}

		bool operator==(_Ty *_interface) const
		{
			return interface_ == _interface;
		}

		bool operator==(const com_ptr& right) const
		{
			return interface_ == right.interface_;
		}

		_Ty* operator->()
		{
			return interface_;
		}

		_Ty& operator*()
		{
			return *interface_;
		}

		_Ty** operator&()
		{
			return &interface_;
		}

		unsigned RefCount()
		{
			return refcount_;
		}

		_Ty* Get()
		{
			return interface_;
		}

		const _Ty* Get() const
		{
			return interface_;
		}

		_Ty** ToCreator()
		{
			if (interface_)
				refcount_ = interface_->Release();
			if (refcount_ == 0)
				interface_ = nullptr;

			internal::template com_ptr_manager<_Ty>::GetInstance()->Add(*this);
			return &interface_;
		}


		com_ptr& operator=(const com_ptr& src)
		{
			// コピー元の参照カウンタを増加
			if (src.interface_)
				refcount_ = src.interface_->AddRef();

			// 自分が保持しているインターフェイスの参照カウンタを減少
			if (interface_)
				interface_->Release();

			// コピー
			interface_ = src.interface_;

			return (*this);
		}
	};

};

