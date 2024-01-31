#define ADD_EXPORTS

#include <DisplayService.hxx>

/*
 * DisplayService class implementation
 */
void GameService::DisplayService::SetWindowName(char *name)
{
    LPCWSTR wName = GameService::charToWChar(name);
    _windowName = wName;
}

void GameService::DisplayService::OpenWindow()
{
    _initMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    _initConVar = (PCONDITION_VARIABLE) malloc(sizeof(CONDITION_VARIABLE));
    _displayMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    InitializeCriticalSection(_initMutex);       //sync the following two threads
    InitializeConditionVariable(_initConVar);
    InitializeCriticalSection(_displayMutex);

    assert(!_msgThreadHdr);    //only one window needed
    _msgThreadHdr = Osal::Thread::createDefaultThread(GameService::_msgReceiveLoop, NULL);

    EnterCriticalSection(_initMutex);
    if(_isDisplaying)
        LeaveCriticalSection(_initMutex);
    else
        SleepConditionVariableCS(_initConVar, _initMutex, INFINITE);        //if the window is not up then wait for it

    _displayThreadHdr = Osal::Thread::createDefaultThread(GameService::_displayLoop, NULL);

    DeleteCriticalSection(_initMutex);
    free(_initMutex);
    free(_initConVar);
    _initMutex = NULL;
    _initConVar = NULL;
}

void GameService::DisplayService::_createWindow()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSW wc = {0};

    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = _windowName;
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = GameService::_windowProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);

    DWORD dwStyle = (WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE);

    RegisterClassW(&wc);
    HWND windowHdlr = CreateWindowW(wc.lpszClassName, _windowName,
          dwStyle,
          _windowPos.GetX(), _windowPos.GetY(), _windowSize.GetX(), _windowSize.GetY(), NULL, NULL, hInstance, NULL);     //open an actual window
    
    _windowHdr = windowHdlr;
}

void GameService::DisplayService::_drawScreen()
{
    assert(_windowHdr);
    HDC hdcDest = GetDC(_windowHdr);

    EnterCriticalSection(DisplayService::_displayMutex);
    std::vector<GameService::GraphicObject *>::iterator objPtrIt;
    std::vector<GameService::GraphicObject *>::iterator objPtrBegin = _rgtdObjs.begin();
    std::vector<GameService::GraphicObject *>::iterator objPtrEnd = _rgtdObjs.end();

    for (objPtrIt = objPtrBegin; objPtrIt != objPtrEnd; objPtrIt++)
    {
        GameService::GraphicObject *objPtr = *objPtrIt;
        objPtr->Draw(hdcDest);
    }
    LeaveCriticalSection(DisplayService::_displayMutex);

    ReleaseDC(_windowHdr, hdcDest);
    //TODO: add mutex to avoid race cond when setting mainBitmap's position
}

void GameService::DisplayService::CloseWindow()
{
    //close window along with the threads associated with it
    PostQuitMessage(0);
    _isDisplaying = false;
    DeleteCriticalSection(_displayMutex);
    free(_displayMutex);
    //TODO: finalize any bitmap handles, etc...
}

void GameService::DisplayService::Register(GameService::GraphicObject *objPtr)
{
    EnterCriticalSection(DisplayService::_displayMutex);
    _rgtdObjs.push_back(objPtr);
    LeaveCriticalSection(DisplayService::_displayMutex);
}

void GameService::DisplayService::Unregister(GameService::GraphicObject *objPtr)
{
    EnterCriticalSection(DisplayService::_displayMutex);
    std::vector<GameService::GraphicObject *>::iterator objPtrIt;
    std::vector<GameService::GraphicObject *>::iterator objPtrBegin = _rgtdObjs.begin();
    std::vector<GameService::GraphicObject *>::iterator objPtrEnd = _rgtdObjs.end();

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

void GameService::DisplayService::RegisterRange(std::vector<GraphicObject *> listObj)
{
    _rgtdObjs.insert(_rgtdObjs.end(), listObj.begin(), listObj.end());
}

void GameService::DisplayService::UnregisterRange(std::vector<GraphicObject *> listObj)
{
    for (int i = 0; i < listObj.size(); i++)
    {
        GraphicObject* obj = listObj.at(i);
        Unregister(obj);
    }
}

/*
 * Standalone functions
 */

DWORD GameService::_msgReceiveLoop(LPVOID thrdArg)
{
    MSG msg;

    EnterCriticalSection(GameService::DisplayService::_initMutex);

    GameService::DisplayService::_createWindow();
    GameService::DisplayService::_isDisplaying = true;

    LeaveCriticalSection(GameService::DisplayService::_initMutex);
    WakeConditionVariable(GameService::DisplayService::_initConVar);

    while (GetMessage(&msg, NULL, 0, 0)) 
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    assert(GameService::DisplayService::_msgThreadHdr);
    CloseHandle(GameService::DisplayService::_msgThreadHdr);

    return 0;   //thread exit
}

DWORD WINAPI GameService::_displayLoop(LPVOID thrdArg)
{
    while(GameService::DisplayService::_isDisplaying)
    {
        GameService::DisplayService::_drawScreen();
        Sleep(1000 / GameService::DisplayService::frameLimit);
    }

    assert(GameService::DisplayService::_displayThreadHdr);
    CloseHandle(GameService::DisplayService::_displayThreadHdr);

    return 0;   //thread exit
}

LRESULT GameService::_windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            GameService::DisplayService::CloseWindow();
            return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

/*
 * init static members
 */
LPCRITICAL_SECTION GameService::DisplayService::_initMutex = NULL;
PCONDITION_VARIABLE GameService::DisplayService::_initConVar = NULL;
LPCRITICAL_SECTION GameService::DisplayService::_displayMutex = NULL;

HANDLE GameService::DisplayService::_msgThreadHdr = NULL;
HANDLE GameService::DisplayService::_displayThreadHdr = NULL;
HWND GameService::DisplayService::_windowHdr = NULL;

LPCWSTR GameService::DisplayService::_windowName = L"Simple Game";
GameService::Vector2 GameService::DisplayService::_windowPos(100, 100);
GameService::Vector2 GameService::DisplayService::_windowSize(700, 540);

bool GameService::DisplayService::_isDisplaying = false;

std::vector<GameService::GraphicObject *> GameService::DisplayService::_rgtdObjs;

unsigned short GameService::DisplayService::frameLimit = 60;