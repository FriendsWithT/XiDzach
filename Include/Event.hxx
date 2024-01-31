#include <windows.h>
#include <Lib.hxx>

#pragma once

namespace GameService
{
    class ADDAPI Event
    {
        private:
            LPCRITICAL_SECTION _waitMutex;
            PCONDITION_VARIABLE _waitConVar;
            bool _waiting;
            void (*_eventCallback)(void*);
        public:
            Event();
            ~Event();
            void Connect(void (*_eventCallback)(void*));
            void Invoke(void *callbackArg);      //run the function connected to this event
            void Wait();
    };
}