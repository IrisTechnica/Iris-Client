
#include "Primitive.h"

#include "../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToStringShex(Primitive value)
{
	switch (value)
	{
	case Primitive::Point :
		return "point";
	case Primitive::Line :
		return "line";
	case Primitive::Triangle :
		return "triangle";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}