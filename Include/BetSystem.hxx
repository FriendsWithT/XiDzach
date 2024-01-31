#include <Common.hxx>
#include <Event.hxx>
#include <fstream>

#define MAX_BALANCE 0xFFFFFFFFFFFFFFFF  //about 18 quintillion :))
#define MAX_WAGER 0xFFFFFFFFFFFFFFFF
#define DEFAULT_BALANCE 1000
#define DAT_FILE "wallet.dat"

namespace SimpleGame
{
    class PlayerWallet
    {
        private:
            UINT64 _balance;
            std::ifstream _ifs;
            std::ofstream _ofs;
        public:
            PlayerWallet();
            ~PlayerWallet();
            void sync();
            void reset();
            void Deposit(UINT64 ammount);
            void Withdraw(UINT64 ammount);

            GameService::Event outOfMoney;
    };

    class BetManager
    {
        public:
            static PlayerWallet _myWallet;
            static UINT64 _wager;

            static void RaiseWager(UINT64 ammount);
            static void LowerWager(UINT64 ammount);
            static UINT64 GetCurrentWager();
    };
}
