#pragma once

#ifdef WAITERCORE_API_EXPORTS
#define WAITERCORE_API __declspec(dllexport)
#else
#define WAITERCORE_API __declspec(dllimport)
#endif
