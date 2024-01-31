#define ADD_EXPORTS

#include <TweenService.hxx>
#include <WinThread.hxx>
#include <math.h>
#include <assert.h>

/*
 * PositionTweenObject implementation
 */

GameService::PositionTweenObject::PositionTweenObject(GraphicObject *obj, Vector2 target, float duration)
: GameService::TweenObject(obj, duration)
{
    this->_obj = obj;
    this->_target = target;

    Vector2 vec = target - obj->GetPosition();
    INT64 squareSum = vec.GetX() * vec.GetX() + vec.GetY() * vec.GetY();
    this->_totalMag = (UINT16) floor(sqrt(squareSum));
    
    INT16 ratio = (duration * 1000) / STD_BREAK_DUARATION;
    this->_movingMag = this->_totalMag / ratio;
}

void GameService::PositionTweenObject::_updateDirection()
{
    Vector2 vec = this->_target - this->_obj->GetPosition();
    INT64 squareSum = vec.GetX() * vec.GetX() + vec.GetY() * vec.GetY();
    UINT16 curMag = (UINT16) floor(sqrt(squareSum));

    INT16 ratio = curMag / this->_movingMag;
    
    INT16 offX = vec.GetX() / ratio;
    INT16 offY = vec.GetY() / ratio;

    this->_direction = Vector2(offX, offY);
}

void GameService::PositionTweenObject::_move()
{
    Vector2 newPos = this->_obj->GetPosition() + this->_direction;
    this->_obj->SetPosition(newPos);
}

/*
 * TweenObject implementation
 */

GameService::TweenObject::TweenObject(GraphicObject *obj, float duration)
{
    this->_obj = obj;
    this->_duration = duration;
    this->_thrdHdr = NULL;
}

void GameService::TweenObject::start()
{
    this->_thrdHdr = Osal::Thread::createDefaultThread(GameService::_tweenRoutine, this);
}

void GameService::TweenObject::_updateDirection()
{

}

void GameService::TweenObject::_move()
{
    
}

DWORD GameService::_tweenRoutine(LPVOID thrdArg)
{
    GameService::TweenObject *twnObj = (GameService::TweenObject *) thrdArg;

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

/*
 * TweenService implementation
 */

GameService::TweenObject* GameService::TweenService::CreatePosTween(GraphicObject *obj, Vector2 target, float duration)
{
    PositionTweenObject *posTwnObj = new PositionTweenObject(obj, target, duration);
    return posTwnObj;
}