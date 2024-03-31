#pragma once

#include <iostream>

#ifdef DEBUG
#define cdebug std::cerr
#else
#define cdebug 0 && std::cerr
#endif
