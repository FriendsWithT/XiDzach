#include <InputService.hxx>

/*
 * Event class implementation
 */

SimpleGame::InputService::Event::Event()
{
    this->_eventCallback = NULL;
    this->_waitMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    this->_waitConVar = (PCONDITION_VARIABLE) malloc(sizeof(CONDITION_VARIABLE));
    InitializeCriticalSection(this->_waitMutex);
    InitializeConditionVariable(this->_waitConVar);
    this->_waiting = false;
}

SimpleGame::InputService::Event::~Event()
{
    DeleteCriticalSection(this->_waitMutex);
    free(this->_waitMutex);
    free(this->_waitConVar);
}

void SimpleGame::InputService::Event::Connect(void (*eventCallback)(int*))
{
    this->_eventCallback = eventCallback;
}

void SimpleGame::InputService::Event::Invoke()
{
    if (this->_eventCallback)
    {
        EnterCriticalSection(this->_waitMutex);
        this->_eventCallback(&(SimpleGame::InputService::_latestInput));
        LeaveCriticalSection(this->_waitMutex);
        
        if (this->_waiting)
            WakeConditionVariable(this->_waitConVar);
    }
}

void SimpleGame::InputService::Event::Wait()
{
    if (this->_eventCallback)
    {
        this->_waiting = true;
        EnterCriticalSection(this->_waitMutex);
        SleepConditionVariableCS(this->_waitConVar, this->_waitMutex, INFINITE);
        LeaveCriticalSection(this->_waitMutex);
        this->_waiting = false;
    }
}

/*
 * InputService class implementation
 */

void SimpleGame::InputService::StartReceiving()
{
    _threadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_receiveLoop, NULL);
}

void SimpleGame::InputService::StopReceiving()
{
    assert(_keyPrsHook);
    PostQuitMessage(0);     //break the msg loop
    UnhookWindowsHookEx(_keyPrsHook);
}

DWORD SimpleGame::_receiveLoop(LPVOID thrdArg)
{
    //when msg comes, _hookCallback will be called
    SimpleGame::InputService::_keyPrsHook = SetWindowsHookEx(WH_KEYBOARD_LL, SimpleGame::_hookCallback, NULL, 0);       //registering the hook
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    assert(SimpleGame::InputService::_threadHdr);
    CloseHandle(SimpleGame::InputService::_threadHdr);

    return 0;       //thread exit
}

LRESULT CALLBACK SimpleGame::_hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

    if (nCode == HC_ACTION)
    {
        SimpleGame::InputService::_latestInput = key->vkCode;
        if (wParam == WM_KEYUP)
            SimpleGame::InputService::keyUp.Invoke();
        else if (wParam == WM_KEYDOWN)
            SimpleGame::InputService::keyDown.Invoke();
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/*
 * init static members
 */
SimpleGame::InputService::Event SimpleGame::InputService::keyUp;
SimpleGame::InputService::Event SimpleGame::InputService::keyDown;

HANDLE SimpleGame::InputService::_threadHdr = NULL;
int SimpleGame::InputService::_latestInput = 0;
HHOOK SimpleGame::InputService::_keyPrsHook = NULL;