#include <InputService.hxx>

/*
 * Event class implementation
 */

SimpleGame::InputService::Event::Event()
{
    this->_eventCallback = NULL;
}

void SimpleGame::InputService::Event::Connect(void (*eventCallback)(int*))
{
    this->_eventCallback = eventCallback;
}

void SimpleGame::InputService::Event::Invoke()
{
    if (this->_eventCallback)
        this->_eventCallback(&(SimpleGame::InputService::_latestInput));
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

DWORD SimpleGame::_receiveLoop(LPVOID inpHdr)
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