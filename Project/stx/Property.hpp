#pragma once
#include <stx/typetraits/typetraits.hpp>

template <typename T>
class Property
{
public:
	Property(T& value) : _value(value) {}
	Property(const Property& ref) : _value(ref._value) {}
	virtual ~Property() {}

	Property<T>& operator = (const Property<T>& ref)
	{
		this->_value = T(ref._value);
		return *this;
	}

protected:
	T&      _value;
};

// Polices
template <typename T>
struct SimplePropertySetter
{
	static void Set(T& value, const T& var) { value = T(var); }
};

template <typename T>
struct SimplePropertyGetter
{
	static const T& Get(const T& value) { return value; }
};

// TODO enable if Ç≈É|ÉCÉìÉ^Ç∆í èÌÇêUÇËï™ÇØÇÈ
template <typename T, class E = stx::accessible<T>::type, class Getter = SimplePropertyGetter<T>, class Setter = SimplePropertySetter <T>>
class WritableProperty : public Property<T>, private Getter, private Setter
{
public:
	WritableProperty(T& value) : Property<T>(value) {}
	WritableProperty(const WritableProperty& ref) : Property<T>(ref) {}
	virtual ~WritableProperty() {}

public:
	operator const T& () const { return this->Get(this->_value); }
	const auto& operator -> () { return stx::accessible<T>::access(&this->Get(this->_value)); }
	operator T() { return this->Get(this->value); }

	WritableProperty<T, Getter, Setter>& operator = (const T& var) { this->Set(this->_value, var); return *this; }
};

template <typename T, class Getter = SimplePropertyGetter<T>>
class ReadOnlyProperty : public Property<T>, private Getter
{
public:
	typedef typename stx::accessible<T>::type value_type;
public:
	ReadOnlyProperty(T& value) : Property<T>(value) {}
	ReadOnlyProperty(const ReadOnlyProperty& ref) : Property<T>(ref) {}
	virtual ~ReadOnlyProperty() {}

public:
	const value_type operator -> () { return stx::accessible<T>::access(this->Get(this->_value)); }
	const T& operator()() const { return this->Get(this->_value); }
	operator const T& () const { return this->Get(this->_value); }
	operator const T() const { return this->Get(this->_value); }
	template<class R>
	bool operator==(R right) const { return this->Get(this->_value) == right; }
};
