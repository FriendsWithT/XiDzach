#define ADD_EXPORTS

#include <GraphicObject.hxx>

/*
 * Vector2 class implementation
 */

GameService::Vector2::Vector2() : _x(0), _y(0) {}

GameService::Vector2::Vector2(INT16 x, INT16 y): _x(x), _y(y) {}

INT16 GameService::Vector2::GetX() const
{
    return this->_x;
}

INT16 GameService::Vector2::GetY() const
{
    return this->_y;
}

void GameService::Vector2::SetX(const INT16 &x)
{
    this->_x = x;
}

void GameService::Vector2::SetY(const INT16 &y)
{
    this->_y = y;
}

GameService::GraphicObject::~GraphicObject()
{

}

void GameService::GraphicObject::Draw(HDC targetHDC)
{

}

GameService::Vector2 GameService::GraphicObject::GetPosition() const
{

}

void GameService::GraphicObject::SetPosition(const GameService::Vector2 &position)
{
    
}