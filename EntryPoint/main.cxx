#include <InputService.hxx>
#include <DisplayService.hxx>
#include <iostream>

void OnKeyPressed(int* ch);

bool running = true;
SimpleGame::Bitmap mainBitmap("../Sandbox/LAND3.BMP", SimpleGame::Vector2(0, 0));

int main()
{
    SimpleGame::InputService::keyUp.Connect(OnKeyPressed);
    SimpleGame::InputService::StartReceiving();

    SimpleGame::DisplayService::mainBitmap = &mainBitmap;
    SimpleGame::DisplayService::OpenWindow();

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
    SimpleGame::Vector2 position = mainBitmap.GetPosition();

    char pressed = (char)*ch;
    switch (pressed)
    {
    case 'A':
        position.SetX(position.GetX() - 1);
        break;
    case 'D':
        position.SetX(position.GetX() + 1);
        break;
    case 'S':
        position.SetY(position.GetY() + 1);
        break;
    case 'W':
        position.SetY(position.GetY() - 1);
        break;
    
    default:
        break;
    }
    
    mainBitmap.SetPosition(position);
    std::cout << "mainBitmap pos is: x[" << mainBitmap.GetPosition().GetX() << "], y[" << mainBitmap.GetPosition().GetY() << "]" << std::endl;

    if (pressed == '0')
    {
        running = false;
        SimpleGame::InputService::StopReceiving();
    }
}