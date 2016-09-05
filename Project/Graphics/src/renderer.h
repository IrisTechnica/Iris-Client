#pragma once
#include <stx/singleton.hpp>
#include <list>
#include "object\drawable_fwd.h"

class Renderer : public stx::singleton<Renderer>
{
public:
	Renderer();
	~Renderer();

private:
	std::list<object::drawable_ptr> rendering_list;
};

