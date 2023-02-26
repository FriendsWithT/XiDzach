#include <DisplayService.hxx>

/*
 * Vector2 class implementation
 */

SimpleGame::Vector2::Vector2() : _x(0), _y(0) {}

SimpleGame::Vector2::Vector2(INT16 x, INT16 y): _x(x), _y(y) {}

INT16 SimpleGame::Vector2::GetX() const
{
    return this->_x;
}

INT16 SimpleGame::Vector2::GetY() const
{
    return this->_y;
}

void SimpleGame::Vector2::SetX(const INT16 &x)
{
    this->_x = x;
}

void SimpleGame::Vector2::SetY(const INT16 &y)
{
    this->_y = y;
}

/*
 * Bitmap class implementation
 */

SimpleGame::Bitmap::Bitmap() : _hBitmap(NULL) {}

SimpleGame::Bitmap::Bitmap(char *fileName, SimpleGame::Vector2 position)
{
    OpenFile(fileName, position);
}

void SimpleGame::Bitmap::OpenFile(char *fileName, SimpleGame::Vector2 position)
{
    LPCWSTR wFileName = SimpleGame::charToWChar(fileName);
    this->_hBitmap = (HBITMAP) LoadImageW(NULL, wFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    this->_fileName = wFileName;

    assert(_hBitmap);   //abort entire program if file not exists
}

SimpleGame::Vector2 SimpleGame::Bitmap::GetPosition() const
{
    return this->_position;
}

HBITMAP SimpleGame::Bitmap::GetHandle() const
{
    return this->_hBitmap;
}

void SimpleGame::Bitmap::SetPosition(const Vector2 &position)
{
    if (position.GetX() >= 0 && position.GetY() >= 0)
        this->_position = position;
}

void SimpleGame::Bitmap::Finalize()
{
    CloseHandle(this->_hBitmap);
    free((LPVOID) this->_fileName);

    this->_hBitmap = NULL;
    this->_fileName = NULL;
}

SimpleGame::Bitmap::~Bitmap()
{
    this->Finalize();
}

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
    InitializeCriticalSection(_initMutex);       //sync the following two threads

    assert(!_msgThreadHdr);    //only one window needed
    _msgThreadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_msgReceiveLoop, NULL);

    Sleep(3);       //wait for msg thread to accquire mutex first
    EnterCriticalSection(_initMutex);
    _isDisplaying = true;
    LeaveCriticalSection(_initMutex);
    _displayThreadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_displayLoop, NULL);

    DeleteCriticalSection(_initMutex);
    free(_initMutex);
    _initMutex = NULL;
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

    RegisterClassW(&wc);
    HWND windowHdlr = CreateWindowW(wc.lpszClassName, _windowName,
          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
          _windowPos.GetX(), _windowPos.GetY(), _windowSize.GetX(), _windowSize.GetY(), NULL, NULL, hInstance, NULL);     //open an actual window
    
    _windowHdr = windowHdlr;
}

void SimpleGame::DisplayService::_drawScreen()
{
    assert(_windowHdr);
    assert(mainBitmap);

    Vector2 bmpPos = mainBitmap->GetPosition();
    HDC hdcDest = GetDC(_windowHdr);
    HDC hdcSrc = CreateCompatibleDC(hdcDest);
    HGDIOBJ oldBitmap = SelectObject(hdcSrc, mainBitmap->GetHandle());
    BITMAP bitmap;

    GetObject(mainBitmap->GetHandle(), sizeof(bitmap), &bitmap);
    BitBlt(hdcDest, bmpPos.GetX(), bmpPos.GetY(), bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY);

    SelectObject(hdcSrc, oldBitmap);
    DeleteDC(hdcSrc);
    ReleaseDC(_windowHdr, hdcDest);
    //TODO: add mutex to avoid race cond when setting mainBitmap's position
}

void SimpleGame::DisplayService::CloseWindow()
{
    //close window along with the threads associated with it
    PostQuitMessage(0);
    _isDisplaying = false;
    //TODO: finalize any bitmap handles, etc...
}

void SimpleGame::DisplayService::RegisterBitmap(SimpleGame::Bitmap *bmpPtr)
{
    _rgtdBmps.push_back(bmpPtr);
}

void SimpleGame::DisplayService::UnregisterBitmap(SimpleGame::Bitmap *bmpPtr)
{
    std::vector<SimpleGame::Bitmap *>::iterator bmpPtrIt;
    std::vector<SimpleGame::Bitmap *>::iterator bmpPtrBegin = _rgtdBmps.begin();
    std::vector<SimpleGame::Bitmap *>::iterator bmpPtrEnd = _rgtdBmps.end();

    for (bmpPtrIt = bmpPtrBegin; bmpPtrIt != bmpPtrBegin; bmpPtrIt++)
    {
        if (*bmpPtrIt == bmpPtr)
        {
            _rgtdBmps.erase(bmpPtrIt);
            return;
        }
    }

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
    LeaveCriticalSection(SimpleGame::DisplayService::_initMutex);

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

LPCWSTR SimpleGame::charToWChar(LPCSTR text)
{
    const size_t size = strlen(text) + 1;
    // wchar_t* wText = new wchar_t[size];
    wchar_t* wText = (wchar_t *) malloc(size * sizeof(wchar_t));
    mbstowcs(wText, text, size);
    return wText;
}

/*
 * init static members
 */
LPCRITICAL_SECTION SimpleGame::DisplayService::_initMutex = NULL;

HANDLE SimpleGame::DisplayService::_msgThreadHdr = NULL;
HANDLE SimpleGame::DisplayService::_displayThreadHdr = NULL;
HWND SimpleGame::DisplayService::_windowHdr = NULL;

LPCWSTR SimpleGame::DisplayService::_windowName = L"Simple Game";
SimpleGame::Vector2 SimpleGame::DisplayService::_windowPos(100, 100);
SimpleGame::Vector2 SimpleGame::DisplayService::_windowSize(700, 540);

bool SimpleGame::DisplayService::_isDisplaying = false;

std::vector<SimpleGame::Bitmap *> SimpleGame::DisplayService::_rgtdBmps;

SimpleGame::Bitmap *SimpleGame::DisplayService::mainBitmap = NULL;
unsigned short SimpleGame::DisplayService::frameLimit = 60;