#pragma once

#ifdef _WIN32
#if defined(asm_EXPORTS)
#define ASSEMBLER_API __declspec(dllexport)
#else
#define ASSEMBLER_API __declspec(dllimport)
#endif
#else
#define ASSEMBLER_API
#endif