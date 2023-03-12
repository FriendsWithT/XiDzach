#include <stdlib.h>
#include <windows.h>
#include <cstdint>

#pragma once

#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

#define INT16 int16_t
#define INT32 int32_t
#define INT64 int64_t

typedef CONDITION_VARIABLE *PCONDITION_VARIABLE;

namespace SimpleGame
{
    inline LPCWSTR charToWChar(LPCSTR text)
    {
        const size_t size = strlen(text) + 1;
        // wchar_t* wText = new wchar_t[size];
        wchar_t* wText = (wchar_t *) malloc(size * sizeof(wchar_t));
        mbstowcs(wText, text, size);
        return wText;
    }
}