#define ADD_EXPORTS

#include <windows.h>
#include <Lib.hxx>
#pragma once

namespace Osal
{
    class ADDAPI Thread
    {
        public:
            static HANDLE createDefaultThread(DWORD (*ThreadProc)(LPVOID), LPVOID lpParameter);
    };
}