#include <Common.hxx>
#include <Bitmap.hxx>
#include <string>
#include <vector>

#define SUIT_TO_PREFIX(suit) (                          \
    (suit == SimpleGame::CardSuit::Clubs)       ? "c" : \
    (suit == SimpleGame::CardSuit::Spades)      ? "s" : \
    (suit == SimpleGame::CardSuit::Diamonds)    ? "d" : \
    (suit == SimpleGame::CardSuit::Hearts)      ? "h" : \
                                                  "\0"  \
)

#define CARD_RANK_MASK 0x00FF
#define CARD_SUIT_MASK 0xFF00

namespace SimpleGame
{
    enum GamePhase
    {
        WELCOME_SCREEN,
        DRAWING_FOUR_CARDS,
        PLAYER_TURN,
        BOT_TURN,
        EVALUATE,
        RESULT_SCREEN
    };

    enum Subject
    {
        ME = 1,
        BOT = -1,
        Neutral = 0
    };

    enum CardFace
    {
        DOWN,
        UP
    };

    enum CardSuit
    {
        Clubs = 1, Spades = 2, Diamonds = 3, Hearts = 4
    };

    enum CardRank
    {
        Ace = 1, Two = 2, Three = 3, Four = 4, Five = 5, Six = 6, Seven = 7,
        Eight = 8, Nine = 9, Ten = 10, Jack = 11, Queen = 12, King = 13
    };

    class Card
    {
        private:
            UINT16 _value;
            GameService::Bitmap _bmp;
            GameService::Bitmap *_backBmp;
        public:
            Card(CardSuit suit, CardRank rank);
            Card(); //back card

            CardFace face;
            void _setBackBmp(GameService::Bitmap *backBmp);
            UINT16 GetValue();
            CardSuit GetSuit();
            CardRank GetRank();
            GameService::Bitmap* GetBmp();
    };

    class GameFlow
    {
        private:
            static Card **_cardPool;
            static UINT16 _cardNum;
            static std::vector<UINT16> _idxPool;

            static GameService::Vector2 _myCardStartPoint;
            static GameService::Vector2 _botCardStartPoint;
            static GameService::Vector2 _cardOffset;

            static void _resetIdxPool();
            static bool _checkBlackjack(Subject player);
        public:
            static std::vector<Card*> myCards;
            static std::vector<Card*> botCards;

            static std::string bmpDir;
            static void LoadCards();
            static Card* DrawOne();
            static std::vector<Card*> DrawFour();
            static void ResetAll();
            static void SetCard(Card* card, Subject player, CardFace face);
            static INT16 GetPoint(Subject player);

            static bool BotDecide();
            static Subject lookForBlackjack();
            static Subject AnnounceWinner();
    };
}