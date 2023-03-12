#include <TweenService.hxx>
#include <WinThread.hxx>
#include <math.h>
#include <assert.h>

SimpleGame::PositionTweenObject::PositionTweenObject(GraphicObject *obj, Vector2 target, float duration)
: SimpleGame::TweenObject(obj, duration)
{
    this->_obj = obj;
    this->_target = target;

    Vector2 vec = target - obj->GetPosition();
    INT64 squareSum = vec.GetX() * vec.GetX() + vec.GetY() * vec.GetY();
    this->_totalMag = (UINT16) floor(sqrt(squareSum));
    
    INT16 ratio = (duration * 1000) / STD_BREAK_DUARATION;
    this->_movingMag = this->_totalMag / ratio;
}

void SimpleGame::PositionTweenObject::_updateDirection()
{
    Vector2 vec = this->_target - this->_obj->GetPosition();
    INT64 squareSum = vec.GetX() * vec.GetX() + vec.GetY() * vec.GetY();
    UINT16 curMag = (UINT16) floor(sqrt(squareSum));

    INT16 ratio = curMag / this->_movingMag;
    
    INT16 offX = vec.GetX() / ratio;
    INT16 offY = vec.GetY() / ratio;

    this->_direction = Vector2(offX, offY);
}

void SimpleGame::PositionTweenObject::_move()
{
    Vector2 newPos = this->_obj->GetPosition() + this->_direction;
    this->_obj->SetPosition(newPos);
}

SimpleGame::TweenObject::TweenObject(GraphicObject *obj, float duration)
{
    this->_obj = obj;
    this->_duration = duration;
    this->_thrdHdr = NULL;
}

void SimpleGame::TweenObject::start()
{
    this->_thrdHdr = Thread::createDefaultThread(SimpleGame::_tweenRoutine, this);
}

void SimpleGame::TweenObject::_updateDirection()
{

}

void SimpleGame::TweenObject::_move()
{
    
}

DWORD SimpleGame::_tweenRoutine(LPVOID thrdArg)
{
    SimpleGame::TweenObject *twnObj = (SimpleGame::TweenObject *) thrdArg;

    INT16 loopCount = (twnObj->_duration * 1000) / STD_BREAK_DUARATION;
    for(int i = 0; i < loopCount; i++)
    {
        twnObj->_updateDirection();
        twnObj->_move();
        Sleep(STD_BREAK_DUARATION);
    }

    assert(twnObj->_thrdHdr);
    CloseHandle(twnObj->_thrdHdr);
    delete twnObj;

    return 0;   //thread exit
}

SimpleGame::TweenObject* SimpleGame::TweenService::CreatePosTween(GraphicObject *obj, Vector2 target, float duration)
{
    PositionTweenObject *posTwnObj = new PositionTweenObject(obj, target, duration);
    return posTwnObj;
}