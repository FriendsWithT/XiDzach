#include <DisplayService.hxx>

/*
 * DisplayService class implementation
 */
void SimpleGame::DisplayService::SetWindowName(char *name)
{
    LPCWSTR wName = SimpleGame::charToWChar(name);
    _windowName = wName;
}

void SimpleGame::DisplayService::OpenWindow()
{
    _initMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    _initConVar = (PCONDITION_VARIABLE) malloc(sizeof(CONDITION_VARIABLE));
    _displayMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(_initMutex);       //sync the following two threads
    InitializeConditionVariable(_initConVar);
    InitializeCriticalSection(_displayMutex);

    assert(!_msgThreadHdr);    //only one window needed
    _msgThreadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_msgReceiveLoop, NULL);

    EnterCriticalSection(_initMutex);
    if(_isDisplaying)
        LeaveCriticalSection(_initMutex);
    else
        SleepConditionVariableCS(_initConVar, _initMutex, INFINITE);        //if the window is not up then wait for it

    _displayThreadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_displayLoop, NULL);

    DeleteCriticalSection(_initMutex);
    free(_initMutex);
    free(_initConVar);
    _initMutex = NULL;
    _initConVar = NULL;
}

void SimpleGame::DisplayService::_createWindow()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSW wc = {0};

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = _windowName;
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = SimpleGame::_windowProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);

    DWORD dwStyle = (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE);

    RegisterClassW(&wc);
    HWND windowHdlr = CreateWindowW(wc.lpszClassName, _windowName,
          dwStyle,
          _windowPos.GetX(), _windowPos.GetY(), _windowSize.GetX(), _windowSize.GetY(), NULL, NULL, hInstance, NULL);     //open an actual window
    
    _windowHdr = windowHdlr;
}

void SimpleGame::DisplayService::_drawScreen()
{
    assert(_windowHdr);
    HDC hdcDest = GetDC(_windowHdr);

    EnterCriticalSection(DisplayService::_displayMutex);
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrIt;
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrBegin = _rgtdObjs.begin();
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrEnd = _rgtdObjs.end();

    for (objPtrIt = objPtrBegin; objPtrIt != objPtrEnd; objPtrIt++)
    {
        SimpleGame::GraphicObject *objPtr = *objPtrIt;
        objPtr->Draw(hdcDest);
    }
    LeaveCriticalSection(DisplayService::_displayMutex);

    ReleaseDC(_windowHdr, hdcDest);
    //TODO: add mutex to avoid race cond when setting mainBitmap's position
}

void SimpleGame::DisplayService::CloseWindow()
{
    //close window along with the threads associated with it
    PostQuitMessage(0);
    _isDisplaying = false;
    DeleteCriticalSection(_displayMutex);
    free(_displayMutex);
    //TODO: finalize any bitmap handles, etc...
}

void SimpleGame::DisplayService::Register(SimpleGame::GraphicObject *objPtr)
{
    EnterCriticalSection(DisplayService::_displayMutex);
    _rgtdObjs.push_back(objPtr);
    LeaveCriticalSection(DisplayService::_displayMutex);
}

void SimpleGame::DisplayService::Unregister(SimpleGame::GraphicObject *objPtr)
{
    EnterCriticalSection(DisplayService::_displayMutex);
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrIt;
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrBegin = _rgtdObjs.begin();
    std::vector<SimpleGame::GraphicObject *>::iterator objPtrEnd = _rgtdObjs.end();

    for (objPtrIt = objPtrBegin; objPtrIt != objPtrEnd; objPtrIt++)
    {
        if (*objPtrIt == objPtr)
        {
            _rgtdObjs.erase(objPtrIt);
            LeaveCriticalSection(DisplayService::_displayMutex);
            return;
        }
    }
    LeaveCriticalSection(DisplayService::_displayMutex);

    assert(0);      //illegal unregister
}

/*
 * Standalone functions
 */

DWORD SimpleGame::_msgReceiveLoop(LPVOID thrdArg)
{
    MSG msg;

    EnterCriticalSection(SimpleGame::DisplayService::_initMutex);

    SimpleGame::DisplayService::_createWindow();
    SimpleGame::DisplayService::_isDisplaying = true;

    LeaveCriticalSection(SimpleGame::DisplayService::_initMutex);
    WakeConditionVariable(SimpleGame::DisplayService::_initConVar);

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    assert(SimpleGame::DisplayService::_msgThreadHdr);
    CloseHandle(SimpleGame::DisplayService::_msgThreadHdr);

    return 0;   //thread exit
}

DWORD WINAPI SimpleGame::_displayLoop(LPVOID thrdArg)
{
    while(SimpleGame::DisplayService::_isDisplaying)
    {
        SimpleGame::DisplayService::_drawScreen();
        Sleep(1000 / SimpleGame::DisplayService::frameLimit);
    }

    assert(SimpleGame::DisplayService::_displayThreadHdr);
    CloseHandle(SimpleGame::DisplayService::_displayThreadHdr);

    return 0;   //thread exit
}

LRESULT SimpleGame::_windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP hBitmap;
    HDC hdc;
    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

    HWND tempHdlr;

    switch(msg) 
    {
        case WM_CREATE:
            break;      

        case WM_PAINT:
            break;

        case WM_DESTROY:
            SimpleGame::DisplayService::CloseWindow();
            return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

/*
 * init static members
 */
LPCRITICAL_SECTION SimpleGame::DisplayService::_initMutex = NULL;
PCONDITION_VARIABLE SimpleGame::DisplayService::_initConVar = NULL;
LPCRITICAL_SECTION SimpleGame::DisplayService::_displayMutex = NULL;

HANDLE SimpleGame::DisplayService::_msgThreadHdr = NULL;
HANDLE SimpleGame::DisplayService::_displayThreadHdr = NULL;
HWND SimpleGame::DisplayService::_windowHdr = NULL;

LPCWSTR SimpleGame::DisplayService::_windowName = L"Simple Game";
SimpleGame::Vector2 SimpleGame::DisplayService::_windowPos(100, 100);
SimpleGame::Vector2 SimpleGame::DisplayService::_windowSize(700, 540);

bool SimpleGame::DisplayService::_isDisplaying = false;

std::vector<SimpleGame::GraphicObject *> SimpleGame::DisplayService::_rgtdObjs;

unsigned short SimpleGame::DisplayService::frameLimit = 60;