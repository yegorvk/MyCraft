//
// Created by egorv on 4/14/2023.
//

#ifndef SHITCRAFT_ARCH_H
#define SHITCRAFT_ARCH_H

#if defined(_WIN32) && !defined(_WIN64)
# define STDCALL __stdcall
#else
# define STDCALL
#endif

#if _MSC_VER && !__INTEL_COMPILER
    #define ALWAYS_INLINE __forceinline
#elif defined(__GNUC__)
    #define ALWAYS_INLINE __attribute__((always_inline))
#else
    #define ALWAYS_INLINE
#endif

#endif //SHITCRAFT_ARCH_H
