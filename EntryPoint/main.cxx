#include <InputService.hxx>
#include <DisplayService.hxx>
#include <TweenService.hxx>
#include <GameFlow.hxx>
#include <iostream>
#include <vector>

void OnKeyPressed(int* ch);

bool running = true;
// SimpleGame::Bitmap mainBitmap("../Sandbox/point.bmp", SimpleGame::Vector2(0, 0));
SimpleGame::Bitmap background("../Sandbox/LAND3.BMP", SimpleGame::Vector2(0, 0));
// SimpleGame::Textbox text("Hello", SimpleGame::Vector2(0, 300), SimpleGame::Vector2(200, 200));

int main()
{
    SimpleGame::InputService::keyUp.Connect(OnKeyPressed);
    SimpleGame::InputService::StartReceiving();

    // SimpleGame::DisplayService::Register(&background);
    // SimpleGame::DisplayService::Register(&mainBitmap);
    // SimpleGame::DisplayService::Register(&text);
    SimpleGame::DisplayService::OpenWindow();
    SimpleGame::DisplayService::Register(&background);
    
    SimpleGame::GameFlow::LoadCards();

    while(running)
    {
        std::cout << "Main loop advanced" << std::endl;
        // Sleep(2000);
        SimpleGame::GameFlow::ResetAll();
        std::vector<SimpleGame::Card*> chosenFour = SimpleGame::GameFlow::DrawFour();
        chosenFour[0]->GetBmp()->SetPosition(SimpleGame::Vector2(0, 0));
        chosenFour[1]->GetBmp()->SetPosition(SimpleGame::Vector2(300, 0));
        chosenFour[2]->GetBmp()->SetPosition(SimpleGame::Vector2(0, 300));
        chosenFour[3]->GetBmp()->SetPosition(SimpleGame::Vector2(300, 300));

        SimpleGame::DisplayService::Register(chosenFour[0]->GetBmp());
        SimpleGame::DisplayService::Register(chosenFour[1]->GetBmp());
        SimpleGame::DisplayService::Register(chosenFour[2]->GetBmp());
        SimpleGame::DisplayService::Register(chosenFour[3]->GetBmp());

        SimpleGame::InputService::keyUp.Wait();

        SimpleGame::DisplayService::Unregister(chosenFour[0]->GetBmp());
        SimpleGame::DisplayService::Unregister(chosenFour[1]->GetBmp());
        SimpleGame::DisplayService::Unregister(chosenFour[2]->GetBmp());
        SimpleGame::DisplayService::Unregister(chosenFour[3]->GetBmp());
        // Sleep(2000);
    }

    return 0;
}

void OnKeyPressed(int* ch)
{
    std::cout << "key pressed: " << *ch << std::endl;
    // SimpleGame::Vector2 target = mainBitmap.GetPosition() + SimpleGame::Vector2(200,0);
    // SimpleGame::TweenObject *twnObj = SimpleGame::TweenService::CreatePosTween(&mainBitmap, target, 0.5);

    char pressed = (char)*ch;
    switch (pressed)
    {
    case 'A':
        // twnObj->start();
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