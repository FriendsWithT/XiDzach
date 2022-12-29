#include <WinThread.hxx>
#include <windows.h>
#include <iostream>

namespace SimpleGame
{
    class InputHandler
    {
        private:
            HANDLE _threadHdr;
            bool _working;
            bool _threadExited;
            int _latestInput;
            void (*_keyPressCallback)(int*);

            friend DWORD WINAPI _receiveLoop(LPVOID inpHdr);     //friend func to match createDefaultThread's arg list
            void Invoke();
        public:
            InputHandler();
            void StartReceiving();
            void StopReceiving();
            void Connect(void (*onKeyPressed)(int*));
    };

    DWORD WINAPI _receiveLoop(LPVOID inpHdr);
}