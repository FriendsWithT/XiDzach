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

namespace SimpleGame
{
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
        public:
            Card(CardSuit suit, CardRank rank);
            Card(); //back card
            UINT16 GetValue();
            GameService::Bitmap* GetBmp();
    };

    class GameFlow
    {
        private:
            static Card **_cardPool;
            static UINT16 _cardNum;
            static std::vector<UINT16> _idxPool;

            static void _resetIdxPool();
        public:
            static std::string bmpDir;
            static void LoadCards();
            static Card* DrawOne();
            static std::vector<Card*> DrawFour();
            static void ResetAll();
    };
}