#include <InputService.hxx>

void GameService::InputService::StartReceiving()
{
    _threadHdr = Osal::Thread::createDefaultThread(GameService::_receiveLoop, NULL);
}

void GameService::InputService::StopReceiving()
{
    assert(_keyPrsHook);
    PostQuitMessage(0);     //break the msg loop
    UnhookWindowsHookEx(_keyPrsHook);
}

DWORD GameService::_receiveLoop(LPVOID thrdArg)
{
    //when msg comes, _hookCallback will be called
    GameService::InputService::_keyPrsHook = SetWindowsHookEx(WH_KEYBOARD_LL, GameService::_hookCallback, NULL, 0);       //registering the hook
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    assert(GameService::InputService::_threadHdr);
    CloseHandle(GameService::InputService::_threadHdr);

    return 0;       //thread exit
}

LRESULT CALLBACK GameService::_hookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

    if (nCode == HC_ACTION)
    {
        GameService::InputService::_latestInput = key->vkCode;
        if (wParam == WM_KEYUP)
            GameService::InputService::keyUp.Invoke(&InputService::_latestInput);
        else if (wParam == WM_KEYDOWN)
            GameService::InputService::keyDown.Invoke(&InputService::_latestInput);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/*
 * init static members
 */
GameService::Event GameService::InputService::keyUp;
GameService::Event GameService::InputService::keyDown;

HANDLE GameService::InputService::_threadHdr = NULL;
int GameService::InputService::_latestInput = 0;
HHOOK GameService::InputService::_keyPrsHook = NULL;