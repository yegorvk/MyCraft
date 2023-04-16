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

#endif //SHITCRAFT_ARCH_H
