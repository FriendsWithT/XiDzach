#include <InputHandler.hxx>
#include <iostream>

void OnKeyPressed(int* ch);

SimpleGame::InputHandler inpHdler;
bool running = true;

int main()
{
    inpHdler.Connect(OnKeyPressed);
    inpHdler.StartReceiving();

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
        inpHdler.StopReceiving();
    }
}