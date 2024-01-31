#include <InputService.hxx>
#include <DisplayService.hxx>
#include <TweenService.hxx>
#include <GameFlow.hxx>
#include <iostream>
#include <vector>

void DrawFourCards(std::vector<GameService::GraphicObject*> *myCardBmps, std::vector<GameService::GraphicObject*> *botCardBmps);
void DrawCard(SimpleGame::Subject player, SimpleGame::CardFace face, std::vector<GameService::GraphicObject*> *bmpBuff);
void RevealBotCards(std::vector<GameService::GraphicObject*> *botCardBmps);
void OnKeyPressed(void* ch);

bool running = true;
SimpleGame::GamePhase currentPhase;
GameService::Bitmap background("../Sandbox/LAND3.BMP", GameService::Vector2(0, 0));

bool stand; //player cancels turn
bool hit;   //player proceeds to draw a card

int main()
{
    GameService::InputService::keyUp.Connect(OnKeyPressed);
    GameService::InputService::StartReceiving();

    GameService::DisplayService::OpenWindow();
    GameService::DisplayService::Register(&background);
    
    SimpleGame::GameFlow::LoadCards();

    while(running)
    {
        //buffers to keep track with bmps used
        std::vector<GameService::GraphicObject*> myCardBmps;
        std::vector<GameService::GraphicObject*> botCardBmps;
        SimpleGame::Subject winner = SimpleGame::Subject::Neutral;

        SimpleGame::GameFlow::ResetAll();

        currentPhase = SimpleGame::GamePhase::DRAWING_FOUR_CARDS;
        DrawFourCards(&myCardBmps, &botCardBmps);

        currentPhase = SimpleGame::GamePhase::EVALUATING;
        winner = SimpleGame::GameFlow::lookForBlackjack();

        if (winner == SimpleGame::Subject::Neutral)
        {
            currentPhase = SimpleGame::GamePhase::PLAYER_TURN;
            stand = false;
            hit = false;
            while (!stand && SimpleGame::GameFlow::myCards.size() < 5)
            {
                GameService::InputService::keyUp.Wait();
                if (hit)
                {
                    DrawCard(SimpleGame::Subject::ME, SimpleGame::CardFace::UP, &myCardBmps);
                    hit = false;
                    std::cout << "player score: " << SimpleGame::GameFlow::GetPoint(SimpleGame::Subject::ME) << std::endl;
                }
            }
            GameService::InputService::keyUp.Wait();

            currentPhase = SimpleGame::GamePhase::BOT_TURN;
            while (SimpleGame::GameFlow::botCards.size() < 5)
            {
                Sleep(1000);
                if ((SimpleGame::GameFlow::BotDecide()) == false)
                    break;
                DrawCard(SimpleGame::Subject::BOT, SimpleGame::CardFace::DOWN, &botCardBmps);
                std::cout << "bot score: " << SimpleGame::GameFlow::GetPoint(SimpleGame::Subject::BOT) << std::endl;
            }
            GameService::InputService::keyUp.Wait();

            currentPhase = SimpleGame::GamePhase::EVALUATING;
            winner = SimpleGame::GameFlow::AnnounceWinner();
        }

        RevealBotCards(&botCardBmps);

        GameService::InputService::keyUp.Wait();

        currentPhase = SimpleGame::GamePhase::RESULT_ANNOUNCE;
        std::cout << "The winner is: " << static_cast<int>(winner) <<std::endl;

        GameService::DisplayService::UnregisterRange(myCardBmps);
        GameService::DisplayService::UnregisterRange(botCardBmps);
    }

    return 0;
}

void DrawFourCards(std::vector<GameService::GraphicObject*> *myCardBmps, std::vector<GameService::GraphicObject*> *botCardBmps)
{
    //subroutine to get 4 cards and distribute them to players. Also register the bitmaps so they appear on screen
    std::vector<SimpleGame::Card*> chosenFour = SimpleGame::GameFlow::DrawFour();

    SimpleGame::GameFlow::SetCard(chosenFour[0], SimpleGame::Subject::ME, SimpleGame::CardFace::UP);
    SimpleGame::GameFlow::SetCard(chosenFour[1], SimpleGame::Subject::ME, SimpleGame::CardFace::UP);
    SimpleGame::GameFlow::SetCard(chosenFour[2], SimpleGame::Subject::BOT, SimpleGame::CardFace::UP);
    SimpleGame::GameFlow::SetCard(chosenFour[3], SimpleGame::Subject::BOT, SimpleGame::CardFace::DOWN);

    myCardBmps->push_back(chosenFour[0]->GetBmp());
    myCardBmps->push_back(chosenFour[1]->GetBmp());
    botCardBmps->push_back(chosenFour[2]->GetBmp());
    botCardBmps->push_back(chosenFour[3]->GetBmp());

    GameService::DisplayService::RegisterRange(*myCardBmps);
    GameService::DisplayService::RegisterRange(*botCardBmps);
}

void DrawCard(SimpleGame::Subject player, SimpleGame::CardFace face, std::vector<GameService::GraphicObject*> *bmpBuff)
{
    //subroutine to draw a card and put it into player's hand
    SimpleGame::Card *chosen = SimpleGame::GameFlow::DrawOne();
    SimpleGame::GameFlow::SetCard(chosen, player, face);
    GameService::DisplayService::Register(chosen->GetBmp());
    bmpBuff->push_back(chosen->GetBmp());
}

void RevealBotCards(std::vector<GameService::GraphicObject*> *botCardBmps)
{
    //subroutine to reveal all bot's cards
    GameService::DisplayService::UnregisterRange(*botCardBmps);
    botCardBmps->clear();
    for (int i = 0; i < SimpleGame::GameFlow::botCards.size(); i++)
    {
        SimpleGame::GameFlow::botCards[i]->face = SimpleGame::CardFace::UP;
        botCardBmps->push_back(SimpleGame::GameFlow::botCards[i]->GetBmp());
    }
    GameService::DisplayService::RegisterRange(*botCardBmps);
}

void OnKeyPressed(void* ch)
{
    int *chCode = (int*)ch;
    // std::cout << "key pressed: " << *chCode << std::endl;

    if (currentPhase == SimpleGame::GamePhase::PLAYER_TURN)
    {
        if (*chCode == 'Q')
        {
            hit = true;
        }
        else if(*chCode == 'E')
        {
            if (SimpleGame::GameFlow::GetPoint(SimpleGame::Subject::ME) > 15)
                stand = true;
        }

    }

    if (*chCode == '0')
    {
        running = false;
        GameService::InputService::StopReceiving();
        exit(0);
    }
}