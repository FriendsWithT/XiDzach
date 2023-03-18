#define ADD_EXPORTS

#include <windows.h>
#include <GraphicObject.hxx>
#include <Lib.hxx>
#include <Common.hxx>

#define STD_BREAK_DUARATION 50

namespace GameService
{
    class ADDAPI TweenObject
    {
        private:
            HANDLE _thrdHdr;
            float _duration;

            virtual void _updateDirection();
            virtual void _move();

            friend DWORD _tweenRoutine(LPVOID thrdArg);
        protected:
            GraphicObject *_obj;
        public:
            TweenObject(GraphicObject *obj, float duration);
            void start();
    };

    class ADDAPI PositionTweenObject : public TweenObject
    {
        private:
            Vector2 _target;
            Vector2 _direction;
            UINT16 _movingMag;
            UINT16 _totalMag;

            void _updateDirection();
            void _move();
        public:
            PositionTweenObject(GraphicObject *obj, Vector2 target, float duration);
    };

    class ADDAPI TweenService
    {
        public:
            static TweenObject* CreatePosTween(GraphicObject *obj, Vector2 target, float duration);
    };

    DWORD _tweenRoutine(LPVOID thrdArg);
}