#include <windows.h>
#include <Lib.hxx>
#pragma once

namespace Osal
{
    typedef DWORD (*ThreadProc)(LPVOID);

    class ADDAPI Thread
    {
        public:
            static HANDLE createDefaultThread(ThreadProc threadProc, LPVOID lpParameter);
    };
}