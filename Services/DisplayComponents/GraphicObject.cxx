#include <GraphicObject.hxx>

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

SimpleGame::GraphicObject::~GraphicObject()
{

}

void SimpleGame::GraphicObject::Draw(HDC targetHDC)
{

}

SimpleGame::Vector2 SimpleGame::GraphicObject::GetPosition() const
{

}

void SimpleGame::GraphicObject::SetPosition(const SimpleGame::Vector2 &position)
{
    
}