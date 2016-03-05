
#include "TessellatorOutputPrimitive.h"

using namespace std;
using namespace HLSLDisassembler;

string HLSLDisassembler::ToStringShex(TessellatorOutputPrimitive value)
{
	switch (value)
	{
	case TessellatorOutputPrimitive::Point :
		return "output_point";
	case TessellatorOutputPrimitive::Line :
		return "output_line";
	case TessellatorOutputPrimitive::TriangleCw :
		return "output_triangle_cw";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}

string HLSLDisassembler::ToStringStat(TessellatorOutputPrimitive value)
{
	switch (value)
	{
	case TessellatorOutputPrimitive::Point :
		return "Point";
	case TessellatorOutputPrimitive::Line :
		return "Line";
	case TessellatorOutputPrimitive::TriangleCw :
		return "Clockwise Triangles";
	default :
		throw runtime_error("Unsupported value: " + to_string((int) value));
	}
}