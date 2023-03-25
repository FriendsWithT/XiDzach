#include <GameFlow.hxx>
#include <GraphicObject.hxx>
#include <cstdlib>
#include <time.h>
#include <assert.h>

SimpleGame::Card::Card(SimpleGame::CardSuit suit, SimpleGame::CardRank rank)
{
    this->_value = (suit << 8) + rank;

    std::string prefix = SUIT_TO_PREFIX(suit);
    std::string postfix = std::to_string(rank);
    if (rank < 10)
        postfix = "0" + postfix;
    
    std::string fileName = GameFlow::bmpDir + prefix + postfix + ".bmp";
    this->_bmp = GameService::Bitmap((char *)fileName.c_str(), GameService::Vector2(0, 0));

    this->_backBmp = NULL;
    this->face = CardFace::UP;
}

SimpleGame::Card::Card()
{
    //virtual cards, used to store back bmp.
    this->_value = 0;

    std::string fileName = GameFlow::bmpDir + "back.bmp";
    this->_bmp = GameService::Bitmap((char *)fileName.c_str(), GameService::Vector2(0, 0));

    this->_backBmp = NULL;
    this->face = CardFace::UP;
}

UINT16 SimpleGame::Card::GetValue()
{
    return this->_value;
}

GameService::Bitmap* SimpleGame::Card::GetBmp()
{
    if (this->face == CardFace::UP)
        return &(this->_bmp);

    assert(_backBmp);
    return this->_backBmp;
}

SimpleGame::CardSuit SimpleGame::Card::GetSuit()
{
    INT16 iCardSuit = (_value & CARD_SUIT_MASK) >> 8;
    return static_cast<CardSuit>(iCardSuit);
}

SimpleGame::CardRank SimpleGame::Card::GetRank()
{
    INT16 iCardRank = _value & CARD_RANK_MASK;
    return static_cast<CardRank>(iCardRank);
}

void SimpleGame::Card::_setBackBmp(GameService::Bitmap *backBmp)
{
    this->_backBmp = backBmp;
}

/*
 * GameFlow implementation
 */

void SimpleGame::GameFlow::LoadCards()
{
    int cardNum = CardSuit::Hearts * CardRank::King + 4;
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
    _cardPool[cardNum - 2] = new Card();     //back card
    _cardPool[cardNum - 3] = new Card();     //back card
    _cardPool[cardNum - 4] = new Card();     //back card
}

void SimpleGame::GameFlow::_resetIdxPool()
{
    _idxPool.clear();

    int max = _cardNum - 4; //chose anything except the back cards
    for (int i = 0; i < max; i++)
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
    myCards.clear();
    botCards.clear();
}

void SimpleGame::GameFlow::SetCard(Card* card, Subject player, CardFace face)
{
    card->face = CardFace::UP;
    if (player == Subject::ME)
    {
        card->GetBmp()->SetPosition(_myCardStartPoint + (_cardOffset * myCards.size()));      
        myCards.push_back(card);
    }
    else
    {
        GameService::Vector2 pos = _botCardStartPoint + (_cardOffset * botCards.size());
        card->GetBmp()->SetPosition(pos);
        if (face == CardFace::DOWN)
        {
            card->face = CardFace::DOWN;
            Card *backCard = _cardPool[_cardNum - botCards.size()];
            backCard->GetBmp()->SetPosition(pos);
            card->_setBackBmp(backCard->GetBmp());
        }
        botCards.push_back(card);
    }
}

INT16 SimpleGame::GameFlow::GetPoint(SimpleGame::Subject player)
{
    std::vector<Card*> *playerCards;
    if (player == Subject::ME)
        playerCards = &myCards;
    else
        playerCards = &botCards;

    INT16 score = 0;
    INT16 pendingAces = 0;
    std::vector<Card*>::iterator cardIt = playerCards->begin();
    std::vector<Card*>::iterator cardEnd = playerCards->end();
    for (cardIt; cardIt != cardEnd; cardIt++)
    {
        Card* card = static_cast<Card*>(*cardIt);
        INT16 cardVal = card->GetRank();
        if (cardVal >= CardRank::Jack)
            cardVal = 10;

        if (cardVal == CardRank::Ace)
        {
            cardVal = 11;
            pendingAces++;
        }
        
        score += cardVal;
    }

    while (score > 21 && pendingAces > 0)
    {
        score -= 10;
        pendingAces--;
    }
    return score;
}

bool SimpleGame::GameFlow::BotDecide()
{
    //true: bot will draw, false: bot will skip
    //bot cheat: it always knows my score
    INT16 botScore = GetPoint(Subject::BOT);
    if (botScore < 16)
        return true;
    else if (botScore >= 21)
        return false;


    INT16 myScore = GetPoint(Subject::ME);
    if (botScore < myScore && myScore <= 21)
        return true;
    return false;
}

bool SimpleGame::GameFlow::_checkBlackjack(Subject player)
{
    std::vector<Card*> *playerCards = &myCards;
    if (player == Subject::BOT) playerCards = &botCards;

    CardRank card1Rank = playerCards->at(0)->GetRank();
    CardRank card2Rank = playerCards->at(1)->GetRank();

    if (card1Rank == CardRank::Ace && card2Rank == CardRank::Ace)
        return true;
    return false;
}

SimpleGame::Subject SimpleGame::GameFlow::lookForBlackjack()
{
    Subject winner = Subject::Neutral;
    INT16 myScore = GetPoint(Subject::ME);
    INT16 botScore = GetPoint(Subject::BOT);

    bool natural = false;
    if (myScore == 21)
    {
        winner = static_cast<Subject>(winner + 1);
        natural = true;
    }
    if (botScore == 21)
    {
        winner = static_cast<Subject>(winner - 1);
        natural = true;
    }
    if (natural)
    {
        if (winner != Subject::Neutral)
            return winner;
    }
    else
        return winner;  //neutral, nobody has blackjack

    if (_checkBlackjack(Subject::ME) == true)
        winner = static_cast<Subject>(winner + 1);
    if (_checkBlackjack(Subject::BOT) == true)
        winner = static_cast<Subject>(winner - 1);

    return winner;
}

SimpleGame::Subject SimpleGame::GameFlow::AnnounceWinner()
{
    Subject winner = Subject::Neutral;
    INT16 myScore = GetPoint(Subject::ME);
    INT16 botScore = GetPoint(Subject::BOT);

    bool busted = false;
    if (myScore > 21)
    {
        winner = static_cast<Subject>(winner - 1);
        busted = true;
    }
    if (botScore > 21)
    {
        winner = static_cast<Subject>(winner + 1);
        busted = true;
    }
    if (busted)
        return winner;

    bool charlie = false;
    if (myCards.size() == 5)
    {
        winner = static_cast<Subject>(winner + 1);
        charlie = true;
    }
    if (botCards.size() == 5)
    {
        winner = static_cast<Subject>(winner - 1);
        charlie = true;
    }
    if (charlie)
        return winner;

    if (myScore > botScore)
        winner = static_cast<Subject>(winner + 1);
    else if (botScore > myScore)
        winner = static_cast<Subject>(winner - 1);
    
    return winner;
}

/*
 * init static members
 */
SimpleGame::Card **SimpleGame::GameFlow::_cardPool = NULL;
UINT16 SimpleGame::GameFlow::_cardNum = 0;
std::vector<UINT16> SimpleGame::GameFlow::_idxPool;
std::string SimpleGame::GameFlow::bmpDir = "../Sandbox/cards/";

GameService::Vector2 SimpleGame::GameFlow::_myCardStartPoint = GameService::Vector2(0, 300);
GameService::Vector2 SimpleGame::GameFlow::_botCardStartPoint = GameService::Vector2(0, 100);
GameService::Vector2 SimpleGame::GameFlow::_cardOffset = GameService::Vector2(100, 0);

std::vector<SimpleGame::Card*> SimpleGame::GameFlow::myCards;
std::vector<SimpleGame::Card*> SimpleGame::GameFlow::botCards;