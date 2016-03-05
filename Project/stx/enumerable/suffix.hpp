#pragma once

//BOOST MACRO ENHANCE
#include <boost/fusion/adapted/struct.hpp>
#define TRANSFORM_STRUCT_MEMBER(ign, name, member) (decltype(name::member), member)
#define FUSION_ADAPT_STRUCT_AUTO(name, members) \
  BOOST_FUSION_ADAPT_STRUCT(name, BOOST_PP_SEQ_FOR_EACH(TRANSFORM_STRUCT_MEMBER, name, members))

#pragma region BOOST_FUSION_ADAPTER

//enumerable2
FUSION_ADAPT_STRUCT_AUTO(char2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(short2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(int2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(long2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(float2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(double2, (x)(y));

FUSION_ADAPT_STRUCT_AUTO(uchar2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(ushort2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(uint2, (x)(y));
FUSION_ADAPT_STRUCT_AUTO(ulong2, (x)(y));

//enumerable3
FUSION_ADAPT_STRUCT_AUTO(char3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(short3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(int3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(long3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(float3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(double3, (x)(y)(z));

FUSION_ADAPT_STRUCT_AUTO(uchar3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(ushort3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(uint3, (x)(y)(z));
FUSION_ADAPT_STRUCT_AUTO(ulong3, (x)(y)(z));

//enumerable4
FUSION_ADAPT_STRUCT_AUTO(char4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(short4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(int4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(long4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(float4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(double4, (x)(y)(z)(w));

FUSION_ADAPT_STRUCT_AUTO(uchar4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(ushort4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(uint4, (x)(y)(z)(w));
FUSION_ADAPT_STRUCT_AUTO(ulong4, (x)(y)(z)(w));

#pragma endregion