#include <WinThread.hxx>
#include <Type.hxx>
#include <windows.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace SimpleGame
{
    class Vector2
    {
        private:
            UINT16 _x, _y;
        public:
            Vector2();
            Vector2(INT16 x, INT16 y);

            INT16 GetX() const;
            INT16 GetY() const;
            void SetX(const INT16 &x);
            void SetY(const INT16 &y);
    };

    class Bitmap
    {
        private:
            Vector2 _position;
            HBITMAP _hBitmap;
            LPCWSTR _fileName;
        public:
            Bitmap();
            Bitmap(char *fileName, Vector2 position);
            ~Bitmap();

            void OpenFile(char *fileName, Vector2 position);
            Vector2 GetPosition() const;
            HBITMAP GetHandle() const;
            void SetPosition(const Vector2 &position);
            void Finalize();
    };

    class DisplayService
    {
        private:
            static LPCRITICAL_SECTION _initMutex;

            static HANDLE _msgThreadHdr;
            static HANDLE _displayThreadHdr;
            static HWND _windowHdr;

            static LPCWSTR _windowName;
            static Vector2 _windowPos;
            static Vector2 _windowSize;

            static bool _isDisplaying;

            static std::vector<Bitmap *> _rgtdBmps;   //registered bitmaps

            static void _createWindow();
            static void _drawScreen();

            friend DWORD WINAPI _msgReceiveLoop(LPVOID thrdArg);
            friend DWORD WINAPI _displayLoop(LPVOID thrdArg);
            friend LRESULT CALLBACK _windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        public:
            static void OpenWindow();
            static void CloseWindow();
            static void SetWindowName(char *name);
            static void RegisterBitmap(Bitmap *bmpPtr);
            static void UnregisterBitmap(Bitmap *bmpPtr);

            static Bitmap *mainBitmap;
            static UINT16 frameLimit;
    };

    DWORD WINAPI _msgReceiveLoop(LPVOID thrdArg);
    DWORD WINAPI _displayLoop(LPVOID thrdArg);
    LRESULT CALLBACK _windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    LPCWSTR charToWChar(LPCSTR text);  //Utility function
}