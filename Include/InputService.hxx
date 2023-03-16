#include <WinThread.hxx>
#include <Common.hxx>
#include <windows.h>
#include <assert.h>

namespace SimpleGame
{
    class InputService
    {
        private:
            class Event
            {
                private:
                    LPCRITICAL_SECTION _waitMutex;
                    PCONDITION_VARIABLE _waitConVar;
                    bool _waiting;
                    void (*_eventCallback)(int*);
                public:
                    Event();
                    ~Event();
                    void Connect(void (*_eventCallback)(int*));
                    void Invoke();      //run the function connected to this event
                    void Wait();
            };

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