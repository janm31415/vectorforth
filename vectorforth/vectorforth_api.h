#pragma once

#ifdef _WIN32
#if defined(vectorforth_EXPORTS)
#  define VECTOR_FORTH_API __declspec(dllexport)
#else
#  define VECTOR_FORTH_API __declspec(dllimport)
#endif
#else
#define VECTOR_FORTH_API
#endif