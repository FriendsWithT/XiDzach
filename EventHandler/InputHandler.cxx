#include <InputHandler.hxx>

/*
 * Event class implementation
 */

SimpleGame::InputHandler::Event::Event()
{
    this->_eventCallback = NULL;
}

void SimpleGame::InputHandler::Event::Connect(void (*eventCallback)(int*))
{
    this->_eventCallback = eventCallback;
}

void SimpleGame::InputHandler::Event::Invoke()
{
    if (this->_eventCallback)
        this->_eventCallback(&(SimpleGame::InputHandler::_latestInput));
}

/*
 * InputHandler class implementation
 */

void SimpleGame::InputHandler::StartReceiving()
{
    _threadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_receiveLoop, NULL);
}

void SimpleGame::InputHandler::StopReceiving()
{
    assert(_keyPrsHook);
    PostQuitMessage(0);     //break the msg loop
    UnhookWindowsHookEx(_keyPrsHook);
}

DWORD SimpleGame::_receiveLoop(LPVOID inpHdr)
{
    //when msg comes, _hookCallback will be called
    SimpleGame::InputHandler::_keyPrsHook = SetWindowsHookEx(WH_KEYBOARD_LL, SimpleGame::_hookCallback, NULL, 0);       //registering the hook
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    assert(SimpleGame::InputHandler::_threadHdr);
    CloseHandle(SimpleGame::InputHandler::_threadHdr);

    return 0;       //thread exit
}

LRESULT CALLBACK SimpleGame::_hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

    if (nCode == HC_ACTION)
    {
        SimpleGame::InputHandler::_latestInput = key->vkCode;
        if (wParam == WM_KEYUP)
            SimpleGame::InputHandler::keyUp.Invoke();
        else if (wParam == WM_KEYDOWN)
            SimpleGame::InputHandler::keyDown.Invoke();
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/*
 * init static members
 */
SimpleGame::InputHandler::Event SimpleGame::InputHandler::keyUp;
SimpleGame::InputHandler::Event SimpleGame::InputHandler::keyDown;

HANDLE SimpleGame::InputHandler::_threadHdr = NULL;
int SimpleGame::InputHandler::_latestInput = 0;
HHOOK SimpleGame::InputHandler::_keyPrsHook = NULL;