#pragma once

#include "graphics.h"

// For console pause: set to 0 later
#define PAUSE 1

#if PAUSE
#define CONSOLE_PAUSE() std::cin.get()
#else
#define CONSOLE_PAUSE()
#endif


