#include <GameFlow.hxx>
#include <GraphicObject.hxx>
#include <cstdlib>
#include <time.h>

SimpleGame::Card::Card(SimpleGame::CardSuit suit, SimpleGame::CardRank rank)
{
    this->_value = (suit << 8) + rank;

    std::string prefix = SUIT_TO_PREFIX(suit);
    std::string postfix = std::to_string(rank);
    if (rank < 10)
        postfix = "0" + postfix;
    
    std::string fileName = GameFlow::bmpDir + prefix + postfix + ".bmp";
    this->_bmp = Bitmap((char *)fileName.c_str(), Vector2(0, 0));
}

SimpleGame::Card::Card()
{
    this->_value = 0;

    std::string fileName = GameFlow::bmpDir + "back.bmp";
    this->_bmp = Bitmap((char *)fileName.c_str(), Vector2(0, 0));
}

UINT16 SimpleGame::Card::GetValue()
{
    return this->_value;
}

SimpleGame::Bitmap* SimpleGame::Card::GetBmp()
{
    return &(this->_bmp);
}

/*
 * GameFlow implementation
 */

void SimpleGame::GameFlow::LoadCards()
{
    int cardNum = CardSuit::Hearts * CardRank::King + 1;
    _cardNum = cardNum;
    _cardPool = new Card*[cardNum];

    for (int suit = CardSuit::Clubs; suit <= CardSuit::Hearts; suit++)
    {
        for (int rank = CardRank::Ace; rank <= CardRank::King; rank++)
        {
            int idx = (suit - 1) * CardRank::King + (rank - 1);
            _cardPool[idx] = new Card((CardSuit)suit, (CardRank)rank);
        }
    }

    _cardPool[cardNum - 1] = new Card();     //back card
}

void SimpleGame::GameFlow::_resetIdxPool()
{
    _idxPool.clear();

    for (int i = 0; i < _cardNum; i++)
    {
        _idxPool.push_back(i);
    }
}

SimpleGame::Card* SimpleGame::GameFlow::DrawOne()
{
    srand(time(0));
    UINT16 randIdx = rand() % _idxPool.size();
    UINT16 chosenCardIdx = _idxPool.at(randIdx);

    _idxPool.erase(_idxPool.begin() + randIdx);

    return _cardPool[chosenCardIdx];
}

std::vector<SimpleGame::Card *> SimpleGame::GameFlow::DrawFour()
{
    std::vector<Card *> rs;

    for (int i = 0; i < 4; i++)
    {
        Card *chosen = DrawOne();
        rs.push_back(chosen);
    }

    return rs;
}

void SimpleGame::GameFlow::ResetAll()
{
    _resetIdxPool();
}

/*
 * init static members
 */
SimpleGame::Card **SimpleGame::GameFlow::_cardPool = NULL;
UINT16 SimpleGame::GameFlow::_cardNum = 0;
std::vector<UINT16> SimpleGame::GameFlow::_idxPool;
std::string SimpleGame::GameFlow::bmpDir = "../Sandbox/cards/";