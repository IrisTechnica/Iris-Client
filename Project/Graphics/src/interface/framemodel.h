#pragma once

#undef interface

namespace interface
{
	class framemodel
	{
		virtual void Update() = 0;
		virtual void Draw() = 0;
	};
};

