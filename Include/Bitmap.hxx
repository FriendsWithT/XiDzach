#include <GraphicObject.hxx>
#include <Lib.hxx>
#pragma once

namespace GameService
{
    class ADDAPI Bitmap : public GraphicObject
    {
        private:
            HBITMAP _hBitmap;
            LPCWSTR _fileName;
            HDC _hMemDC;
            bool _finalized;

            void _generateMemDC();
        public:
            Bitmap();
            Bitmap(char *fileName, Vector2 position);
            ~Bitmap();

            void OpenFile(char *fileName);
            Vector2 GetPosition() const;
            HBITMAP GetHandle() const;
            void SetPosition(const Vector2 &position);
            void Finalize();

            virtual void Draw(HDC targetHDC);
    };
}