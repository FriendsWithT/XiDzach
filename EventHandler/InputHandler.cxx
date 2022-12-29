#include <InputHandler.hxx>

SimpleGame::InputHandler::InputHandler()
{
}

void SimpleGame::InputHandler::StartReceiving()
{
    this->_working = true;

    HANDLE _threadHdr = SimpleGame::Thread::createDefaultThread(SimpleGame::_receiveLoop, this);
    this->_threadHdr = _threadHdr;
}

void SimpleGame::InputHandler::StopReceiving()
{
    this->_working = false;
    while(!this->_threadExited);
    CloseHandle(this->_threadHdr);
}

void SimpleGame::InputHandler::Connect(void (*onKeyPressed)(int*))
{
    this->_keyPressCallback = onKeyPressed;
}

void SimpleGame::InputHandler::Invoke()
{
    this->_keyPressCallback(&(this->_latestInput));
}

DWORD SimpleGame::_receiveLoop(LPVOID inpHdr)
{
    SimpleGame::InputHandler *inputHandler = (SimpleGame::InputHandler*) inpHdr;
    while(inputHandler->_working)
    {
        std::cout << "Thread waiting for input" << std::endl;
        int input = std::cin.get();
        inputHandler->_latestInput = input;
        inputHandler->Invoke();
    }
    inputHandler->_threadExited = true;

    return 0;
}