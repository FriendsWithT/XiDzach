#include <windows.h>

namespace SimpleGame
{
    class Thread
    {
        public:
            static HANDLE createDefaultThread(DWORD (*ThreadProc)(LPVOID), LPVOID lpParameter);
    };
}