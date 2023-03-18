#include <InputService.hxx>
#include <DisplayService.hxx>
#include <TweenService.hxx>
#include <GameFlow.hxx>
#include <iostream>
#include <vector>

void OnKeyPressed(void* ch);

bool running = true;
GameService::Bitmap background("../Sandbox/LAND3.BMP", GameService::Vector2(0, 0));

int main()
{
    GameService::InputService::keyUp.Connect(OnKeyPressed);
    GameService::InputService::StartReceiving();

    GameService::DisplayService::OpenWindow();
    GameService::DisplayService::Register(&background);
    
    SimpleGame::GameFlow::LoadCards();

    while(running)
    {
        std::cout << "Main loop advanced" << std::endl;

        SimpleGame::GameFlow::ResetAll();
        std::vector<SimpleGame::Card*> chosenFour = SimpleGame::GameFlow::DrawFour();
        chosenFour[0]->GetBmp()->SetPosition(GameService::Vector2(0, 0));
        chosenFour[1]->GetBmp()->SetPosition(GameService::Vector2(300, 0));
        chosenFour[2]->GetBmp()->SetPosition(GameService::Vector2(0, 300));
        chosenFour[3]->GetBmp()->SetPosition(GameService::Vector2(300, 300));

        std::vector<GameService::GraphicObject*> cardBmps{chosenFour[0]->GetBmp(), chosenFour[1]->GetBmp(),
                                                         chosenFour[2]->GetBmp(), chosenFour[3]->GetBmp()};

        GameService::DisplayService::RegisterRange(cardBmps);

        GameService::InputService::keyUp.Wait();

        GameService::DisplayService::UnregisterRange(cardBmps);

    }

    return 0;
}

void OnKeyPressed(void* ch)
{
    int *chCode = (int*)ch;
    std::cout << "key pressed: " << *chCode << std::endl;

    char pressed = (char)*chCode;
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
        GameService::InputService::StopReceiving();
    }
}