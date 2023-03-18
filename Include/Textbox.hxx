#define ADD_EXPORTS

#include <GraphicObject.hxx>
#include <Lib.hxx>
#pragma once

namespace GameService
{
    class ADDAPI Textbox : public GraphicObject
    {
        private:
            LPCWSTR _wText;
            int _textLen;
            Vector2 _boxSize;
            Vector2 _downRight;
            LPRECT _pRect;
            bool _finalized;
        public:
            Textbox();
            Textbox(char *text, Vector2 position, Vector2 boxSize);
            ~Textbox();

            Vector2 GetPosition() const;
            LPRECT getRectPtr() const;
            void SetPosition(const Vector2 &position);
            void SetText(const char *text);
            void SetSize(const Vector2 &boxSize);
            void Finalize();

            virtual void Draw(HDC targetHDC);
    };
}