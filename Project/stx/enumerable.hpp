#pragma once
#include <vector>
#include <boost/fusion/adapted.hpp>
#include <boost/serialization/serialization.hpp>

template<class _T>
struct enumerable2
{
	union {
		struct {
			_T x, y;
		};
		_T r[2];
	};
public:
	typedef _T content_type;
	enumerable2()
		: x(0), y(0)
	{}

	template<class _R>
	enumerable2(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
	}

	explicit enumerable2(const _T *right)
		: x(right[0]), y(right[1])
	{}

	template<class _R>
	enumerable2& operator=(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
		return *this;
	}

	_T operator[](unsigned int index)
	{
		return r[index];
	}

	const _T operator[](unsigned int index) const
	{
		return r[index];
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		static_cast<void>(version);
		ar & x;
		ar & y;
	}
};

typedef enumerable2<char>   char2;
typedef enumerable2<short>  short2;
typedef enumerable2<int>    int2;
typedef enumerable2<long>   long2;
typedef enumerable2<float>  float2;
typedef enumerable2<double> double2;

typedef enumerable2<unsigned char>   uchar2;
typedef enumerable2<unsigned short>  ushort2;
typedef enumerable2<unsigned int>    uint2;
typedef enumerable2<unsigned long>   ulong2;

template<class _T>
struct enumerable3
{
	union {
		struct {
			_T x, y, z;
		};
		_T r[3];
	};
public:
	typedef _T content_type;
	enumerable3()
		: x(0), y(0), z(0)
	{}
	template<class _R>
	enumerable3(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
		z = static_cast<_T>(arr[2]);
	}

	explicit enumerable3(const _T *right)
		: x(right[0]), y(right[1]), z(right[2])
	{}

	template<class _R>
	enumerable3& operator=(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
		z = static_cast<_T>(arr[2]);
		return *this;
	}

	_T operator[](unsigned int index)
	{
		return r[index];
	}

	const _T operator[](unsigned int index) const
	{
		return r[index];
	}

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, unsigned int version)
	{
		static_cast<void>(version);
		ar & x;
		ar & y;
		ar & z;
	}

};

typedef enumerable3<char>   char3;
typedef enumerable3<short>  short3;
typedef enumerable3<int>    int3;
typedef enumerable3<long>   long3;
typedef enumerable3<float>  float3;
typedef enumerable3<double> double3;

typedef enumerable3<unsigned char>   uchar3;
typedef enumerable3<unsigned short>  ushort3;
typedef enumerable3<unsigned int>    uint3;
typedef enumerable3<unsigned long>   ulong3;

template<class _T>
struct enumerable4
{
	union {
		struct {
			_T x, y, z, w;
		};
		_T r[4];
	};
public:
	typedef _T content_type;
	enumerable4()
		: x(0), y(0), z(0), w(0)
	{}
	template<class _R>
	enumerable4(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
		z = static_cast<_T>(arr[2]);
		w = static_cast<_T>(arr[3]);
	}

	enumerable4(const unsigned int &right)
	{
		x = (0xff000000 & right) >> 24;
		y = (0x00ff0000 & right) >> 16;
		z = (0x0000ff00 & right) >> 8;
		w = (0x000000ff & right) >> 0;
	}

	explicit enumerable4(const _T *right)
		: x(right[0]), y(right[1]), z(right[2]), w(right[3])
	{}

	template<class _R>
	enumerable4& operator=(std::initializer_list<_R> list)
	{
		std::vector<_R> arr; for (const auto &var : list) { arr.push_back(var); }

		x = static_cast<_T>(arr[0]);
		y = static_cast<_T>(arr[1]);
		z = static_cast<_T>(arr[2]);
		w = static_cast<_T>(arr[3]);
		return *this;
	}


	_T operator[](unsigned int index)
	{
		return r[index];
	}

	const _T operator[](unsigned int index) const
	{
		return r[index];
	}

};

typedef enumerable4<char>   char4;
typedef enumerable4<short>  short4;
typedef enumerable4<int>    int4;
typedef enumerable4<long>   long4;
typedef enumerable4<float>  float4;
typedef enumerable4<double> double4;

typedef enumerable4<unsigned char>   uchar4;
typedef enumerable4<unsigned short>  ushort4;
typedef enumerable4<unsigned int>    uint4;
typedef enumerable4<unsigned long>   ulong4;

#include "enumerable/suffix.hpp"