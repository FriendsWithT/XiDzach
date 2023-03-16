#include <WinThread.hxx>
#include <GraphicObject.hxx>
#include <Bitmap.hxx>
#include <Textbox.hxx>
#include <Common.hxx>
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <vector>


namespace SimpleGame
{
    class DisplayService
    {
        private:
            static LPCRITICAL_SECTION _initMutex;
            static PCONDITION_VARIABLE _initConVar;
            static LPCRITICAL_SECTION _displayMutex;

            static HANDLE _msgThreadHdr;
            static HANDLE _displayThreadHdr;
            static HWND _windowHdr;

            static LPCWSTR _windowName;
            static Vector2 _windowPos;
            static Vector2 _windowSize;

            static bool _isDisplaying;

            static std::vector<GraphicObject *> _rgtdObjs;   //registered graphic objects

            static void _createWindow();
            static void _drawScreen();

            friend DWORD WINAPI _msgReceiveLoop(LPVOID thrdArg);
            friend DWORD WINAPI _displayLoop(LPVOID thrdArg);
            friend LRESULT CALLBACK _windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        public:
            static void OpenWindow();
            static void CloseWindow();
            static void SetWindowName(char *name);
            static void Register(GraphicObject *objPtr);
            static void Unregister(GraphicObject *objPtr);

            static UINT16 frameLimit;
    };

    DWORD WINAPI _msgReceiveLoop(LPVOID thrdArg);
    DWORD WINAPI _displayLoop(LPVOID thrdArg);
    LRESULT CALLBACK _windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}