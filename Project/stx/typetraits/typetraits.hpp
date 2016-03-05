#pragma once
#include <xstddef>
#include <boost/mpl/or.hpp>

/////////////////////////////////////////////////////
// Begin of stx
/////////////////////////////////////////////////////
namespace stx{
	using namespace std;

	extern void * enabler;
	template<bool B, typename T = void> using disable_if = std::enable_if < !B, T >;


	/////////////////////////////////////////////////////
	//
	// Type Traits Extention And Et cetra (of STL)
	// autor : Yuki Nakashima
	//
	// using techniques : Template Template Parameter, SFINAE
	//
	/////////////////////////////////////////////////////

	// Stop the compile,cause an error when false
	template<class _Ty, typename enable_if<_Ty::value >::type *& = enabler>
	struct compile_error
	{
	};

	template<class _Ty>
	struct _Function_result
		//: complie_error<true_type>
	{
		typedef void type;
	};

	template<class _Ty>
	struct _Function_result< _Ty(*)() >
		: remove_volatile < _Ty >
	{
	};

	template<class _Ty1, class ..._Ty2>
	struct _Function_result< _Ty1(*)(_Ty2 ...) >
		: remove_volatile < _Ty1 >
	{
	};

	template<class _Ty1, class _Ty2>
	struct _Memberfunction_result
		//: complie_error<true_type>
	{
		typedef void type;
	};

	template<class _Ty1, class _Ty2>
	struct _Memberfunction_result< _Ty1, _Ty2(_Ty1::*)() >
		: remove_volatile < _Ty2 >
	{
	};

	template<class _Ty1, class _Ty2>
	struct _Memberfunction_result< _Ty1, _Ty2(_Ty1::*)() const >
		: remove_volatile < _Ty2 >
	{
	};


	// temporary impl
	namespace detail{
		template<class U>
		class has_arrow_impl{
			typedef char yes_type;
			typedef struct { char c[8]; } no_type;

			template< typename U >
			static yes_type check(U *, decltype(&U::operator->) * = NULL);


			template< typename U >
			static no_type check(...);
		public:
			static bool const value = sizeof(check<U>(NULL)) == sizeof(yes_type);
		};
	}
	template<typename T>
	struct has_arrow : boost::mpl::bool_ < detail::has_arrow_impl<T>::value > {};

	template<class _Ty>
	struct _Is_objective_pointer
		: false_type
	{
	};

	// Technical comments : used on Template Template Parameter
	template<template<class> class _Ty1, class _Ty2>
	struct _Is_objective_pointer<_Ty1<_Ty2>>
		: _Cat_base < has_arrow<_Ty1<_Ty2>>::value >
	{
	};

	template<class _Ty>
	struct _Is_objective_pointer < _Ty * >
		: _Cat_base < !is_member_object_pointer<_Ty *>::value
		&& !is_member_function_pointer<_Ty *>::value >
	{
	};

	template<class _Ty>
	struct is_objective_pointer
		: _Is_objective_pointer < typename remove_cv<_Ty>::type >
	{    // determine whether _Ty is a pointer inicludes objects such as shared_ptr 
	};

	template<class _Ty>
	struct _Remove_objective_pointer
	{
		typedef _Ty type;
	};

	template<template<class T, class Allocator = std::allocator<T>> class _Ty1, class _Ty2>
	struct _Remove_objective_pointer < _Ty1<_Ty2> >
	{
		typedef _Ty2 type;
	};


#define _REMOVE_OBJ_POINTER(CV_OPT) \
	template<class _Ty> \
	struct _Remove_objective_pointer< _Ty *CV_OPT> \
			{ \
	typedef _Ty type; \
			};

	_CLASS_DEFINE_CV(_REMOVE_OBJ_POINTER)
#undef _REMOVE_OBJ_POINTER

		template<class _Ty>
	struct remove_objective_pointer
		: _Remove_objective_pointer < _Ty >
	{
	};

	template <class _Ty1, class _Ty2>
	struct same_if : std::enable_if < std::is_same<_Ty1, _Ty2>::value >
	{
	};

	namespace detail
	{

		template<class _Cs1,class ... _Cs2>
		struct __placeholder{};

		template<bool,class,class>
		struct contains_same_if_detail
		{
			//nothing
		};
			
		template <bool _Sa,class _Ty1, template <class... > class _Ty2, class _Bd>
		struct contains_same_if_detail <_Sa,_Ty1,_Ty2< _Bd> >:
			boost::mpl::or_<
				boost::mpl::bool_<std::is_same<_Ty1,_Bd>::value>,
				boost::mpl::bool_<_Sa>
			>
		{
		};

		template <bool _Sa, class _Ty1, template <class... > class _Ty2, class _Hd, class... _Bd>
		struct contains_same_if_detail <_Sa, _Ty1, _Ty2< _Hd, _Bd... > >
		{
			static const bool value =
				contains_same_if_detail<
					boost::mpl::or_ <
						boost::mpl::bool_<std::is_same<_Ty1, _Hd>::value>,
						boost::mpl::bool_ < _Sa >
					> ::value,
					_Ty1,
					__placeholder<_Bd...>
				>::value;
		};

	};

	template <class _Ty1, class ... _TyN>
	struct contains_same_if : std::enable_if< detail::contains_same_if_detail<false,_Ty1, detail::__placeholder<_TyN...> >::value >
	{

	};


}
/////////////////////////////////////////////////////
// End of stx
/////////////////////////////////////////////////////
