#include <windows.h>
#pragma once

namespace SimpleGame
{
    class Thread
    {
        public:
            static HANDLE createDefaultThread(DWORD (*ThreadProc)(LPVOID), LPVOID lpParameter);
    };
}