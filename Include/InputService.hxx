#include <WinThread.hxx>
#include <Common.hxx>
#include <Event.hxx>
#include <Lib.hxx>
#include <windows.h>
#include <assert.h>

namespace GameService
{
    class ADDAPI InputService
    {
        private:
            static HANDLE _threadHdr;
            static int _latestInput;
            static HHOOK _keyPrsHook;

            friend DWORD WINAPI _receiveLoop(LPVOID thrdArg);     //friend func to match createDefaultThread's arg list
            friend LRESULT CALLBACK _hookCallback(int nCode, WPARAM wParam, LPARAM lParam);
        public:
            static void StartReceiving();
            static void StopReceiving();

            static Event keyUp;
            static Event keyDown;
    };

    DWORD WINAPI _receiveLoop(LPVOID thrdArg);
    LRESULT CALLBACK _hookCallback(int nCode, WPARAM wParam, LPARAM lParam);
}