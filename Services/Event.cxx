#include <Event.hxx>

GameService::Event::Event()
{
    this->_eventCallback = NULL;
    this->_waitMutex = (LPCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
    this->_waitConVar = (PCONDITION_VARIABLE) malloc(sizeof(CONDITION_VARIABLE));
    InitializeCriticalSection(this->_waitMutex);
    InitializeConditionVariable(this->_waitConVar);
    this->_waiting = false;
}

GameService::Event::~Event()
{
    DeleteCriticalSection(this->_waitMutex);
    free(this->_waitMutex);
    free(this->_waitConVar);
}

void GameService::Event::Connect(void (*eventCallback)(void*))
{
    this->_eventCallback = eventCallback;
}

void GameService::Event::Invoke(void *callbackArg)
{
    if (this->_eventCallback)
    {
        EnterCriticalSection(this->_waitMutex);
        this->_eventCallback(callbackArg);
        LeaveCriticalSection(this->_waitMutex);
        
        if (this->_waiting)
            WakeConditionVariable(this->_waitConVar);
    }
}

void GameService::Event::Wait()
{
    if (this->_eventCallback)
    {
        this->_waiting = true;
        EnterCriticalSection(this->_waitMutex);
        SleepConditionVariableCS(this->_waitConVar, this->_waitMutex, INFINITE);
        LeaveCriticalSection(this->_waitMutex);
        this->_waiting = false;
    }
}