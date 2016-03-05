#pragma once
#include <string>
#include <set>
#include "unicode.hpp"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>

namespace stx
{
using namespace std;

// can speed up when use on ordered map 
template<class Key, class Comp = less<Key> >
class basic_symbol {
public:
	typedef unsigned long            hash_type;
	typedef Key                      key_type;

	basic_symbol()
	{
		k = &(*key_pool().insert(key_type()).first);
	}
	basic_symbol(const key_type& key)
	{
		k = &(*key_pool().insert(key).first);
	}
	basic_symbol(const basic_symbol& sym)
		: k(sym.k) {}

	basic_symbol& operator=(const basic_symbol& sym)
	{
		k = sym.k; return *this;
	}
	basic_symbol& operator=(const key_type& key)
	{
		k = &(*key_pool().insert(key).first); return *this;
	}

	const key_type& key() const { return *k; }
	hash_type hash() const { return hash_type(k); }

private:
	typedef std::set<key_type, Comp> pool_allocator;
	static pool_allocator& key_pool();
	const key_type* k;

private:
	friend boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, unsigned int /*version*/)
	{
		ar & key_pool();
	}

};


template<class Key, class Comp>
typename basic_symbol<Key, Comp>::pool_allocator&
basic_symbol<Key, Comp>::key_pool() {
	static pool_allocator pool;
	return pool;
}

template<class K, class C>
inline bool operator==(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() == y.hash();
}

template<class K, class C>
inline bool operator!=(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() != y.hash();
}

template<class K, class C>
inline bool operator<(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() < y.hash();
}

template<class K, class C>
inline bool operator<=(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() <= y.hash();
}

template<class K, class C>
inline bool operator>(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() > y.hash();
}

template<class K, class C>
inline bool operator>=(const basic_symbol<K, C>& x, const basic_symbol<K, C>& y)
{
	return x.hash() >= y.hash();
}

typedef basic_symbol<::tstring> tstring_symbol;


};

//std hask definition
namespace std
{
template<class Key>
struct hash < stx::basic_symbol<Key> >
{
	std::size_t operator()(stx::basic_symbol<Key> const & key) const
	{
		return key.hash();
	}
};
};

