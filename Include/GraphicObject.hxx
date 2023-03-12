#include <Common.hxx>
#include <windows.h>

#pragma once

namespace SimpleGame
{
    enum Color
    {
        rgbRed = 0x000000FF,
        rgbGreen = 0x0000FF00,
        rgbBlue = 0x00FF0000,
        rgbBlack = 0x00000000,
        rgbWhite = 0x00FFFFFF,
    };

    class Vector2
    {
        private:
            INT16 _x, _y;
        public:
            Vector2();
            Vector2(INT16 x, INT16 y);

            INT16 GetX() const;
            INT16 GetY() const;
            void SetX(const INT16 &x);
            void SetY(const INT16 &y);

            Vector2 operator - (Vector2 other)
            {
                return Vector2(this->_x - other.GetX(), this->_y - other.GetY());
            }

            Vector2 operator + (Vector2 other)
            {
                return Vector2(this->_x + other.GetX(), this->_y + other.GetY());
            }
    };

    class GraphicObject
    {
        protected:
            Vector2 _position;
        public:
            virtual void Draw(HDC targetHDC);

            virtual Vector2 GetPosition() const;
            virtual void SetPosition(const Vector2 &position);

            virtual ~GraphicObject();
    };
}