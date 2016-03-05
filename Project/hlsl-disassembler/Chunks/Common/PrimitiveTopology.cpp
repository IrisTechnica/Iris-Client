
#include "PrimitiveTopology.h"

#include "../../Util/Decoder.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToStringShex(PrimitiveTopology value)
{
	switch (value)
	{
	case PrimitiveTopology::PointList :
		return "pointlist";
	case PrimitiveTopology::LineList :
		return "linelist";
	case PrimitiveTopology::LineStrip :
		return "linestrip";
	case PrimitiveTopology::TriangleList :
		return "trianglelist";
	case PrimitiveTopology::TriangleStrip :
		return "trianglestrip";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}