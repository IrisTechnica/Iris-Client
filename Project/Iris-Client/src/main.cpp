
#define DEBUGCONSOLE_SHOW

#include <stx/config.hpp>
#include <Graphics/config.hpp>
#include <stx/entrypoint.hpp>

#include "core.h"

ENTRYPOINT
{
	Core core;
	core.Run();
}