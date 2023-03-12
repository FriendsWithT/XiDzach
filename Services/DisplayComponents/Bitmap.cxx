#include <Bitmap.hxx>
#include <assert.h>

SimpleGame::Bitmap::Bitmap() : _hBitmap(NULL), _hMemDC(NULL), _finalized(false) {}

SimpleGame::Bitmap::Bitmap(char *fileName, SimpleGame::Vector2 position)
{
    this->OpenFile(fileName);
    this->SetPosition(position);
    this->_finalized = false;

}

void SimpleGame::Bitmap::OpenFile(char *fileName)
{
    LPCWSTR wFileName = SimpleGame::charToWChar(fileName);
    this->_hBitmap = (HBITMAP) LoadImageW(NULL, wFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    this->_fileName = wFileName;

    assert(_hBitmap);   //abort entire program if file not exists
}

void SimpleGame::Bitmap::_generateMemDC()
{

}

void SimpleGame::Bitmap::Draw(HDC targetHDC)
{
    Vector2 bmpPos = this->GetPosition();
    HDC hdcSrc = CreateCompatibleDC(targetHDC);
    HGDIOBJ oldBitmap = SelectObject(hdcSrc, this->GetHandle());
    BITMAP bitmap;

    GetObject(this->GetHandle(), sizeof(bitmap), &bitmap);
    BitBlt(targetHDC, bmpPos.GetX(), bmpPos.GetY(), bitmap.bmWidth, bitmap.bmHeight, hdcSrc, 0, 0, SRCCOPY);

    SelectObject(hdcSrc, oldBitmap);
    DeleteDC(hdcSrc);
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
    this->_finalized = true;
}

SimpleGame::Bitmap::~Bitmap()
{
    if (!this->_finalized) this->Finalize();
}