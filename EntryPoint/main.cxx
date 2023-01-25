#include <InputService.hxx>
#include <iostream>

void OnKeyPressed(int* ch);

bool running = true;

int main()
{
    SimpleGame::InputService::keyUp.Connect(OnKeyPressed);
    SimpleGame::InputService::StartReceiving();

    while(running)
    {
        std::cout << "Main loop advanced" << std::endl;
        Sleep(2000);
    }

    return 0;
}

void OnKeyPressed(int* ch)
{
    std::cout << "key pressed: " << *ch << std::endl;

    char zero = '0';
    if (*ch == (int)zero)
    {
        running = false;
        SimpleGame::InputService::StopReceiving();
    }
}