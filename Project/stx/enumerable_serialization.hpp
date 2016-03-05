#pragma once
#include "enumerable.hpp"

#include <boost/serialization/serialization.hpp>

namespace boost {
namespace serialization {

template<class Archive,class _Ty>
void serialization(Archive & ar,enumerable2<_Ty> & d, unsigned int version) {
	ar & d.r;
}

template<class Archive, class _Ty>
void serialization(Archive & ar, enumerable3<_Ty> & d, unsigned int version) {
	ar & d.r;
}

template<class Archive, class _Ty>
void serialization(Archive & ar, enumerable4 <_Ty> & d, unsigned int version) {
	ar & d.r;
}


}
}