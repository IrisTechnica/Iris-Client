#pragma once
#include <memory>
#include "../interface/framemodel.h"
#include "../renderer.h"

namespace object
{
	class drawable : public virtual interface::framemodel
	{

	};

	using drawable_ptr = std::shared_ptr<drawable>;
};