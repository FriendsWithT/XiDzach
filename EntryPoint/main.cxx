#include <InputService.hxx>
#include <DisplayService.hxx>
#include <TweenService.hxx>
#include <iostream>

void OnKeyPressed(int* ch);

bool running = true;
SimpleGame::Bitmap mainBitmap("../Sandbox/point.bmp", SimpleGame::Vector2(0, 0));
SimpleGame::Bitmap background("../Sandbox/LAND3.BMP", SimpleGame::Vector2(0, 0));
SimpleGame::Textbox text("Hello", SimpleGame::Vector2(0, 300), SimpleGame::Vector2(200, 200));

int main()
{
    SimpleGame::InputService::keyUp.Connect(OnKeyPressed);
    SimpleGame::InputService::StartReceiving();

    SimpleGame::DisplayService::Register(&background);
    SimpleGame::DisplayService::Register(&mainBitmap);
    SimpleGame::DisplayService::Register(&text);
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
    SimpleGame::Vector2 target = mainBitmap.GetPosition() + SimpleGame::Vector2(200,0);
    SimpleGame::TweenObject *twnObj = SimpleGame::TweenService::CreatePosTween(&mainBitmap, target, 0.5);

    char pressed = (char)*ch;
    switch (pressed)
    {
    case 'A':
        twnObj->start();
        break;
    case 'D':
        break;
    case 'S':
        break;
    case 'W':
        break;
    
    default:
        break;
    }

    if (pressed == '0')
    {
        running = false;
        SimpleGame::InputService::StopReceiving();
    }
}