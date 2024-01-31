#include <BetSystem.hxx>
#include <assert.h>

/*
 * PlayerWallet implementation
 */

SimpleGame::PlayerWallet::PlayerWallet()
{
    this->_ofs.open(DAT_FILE, std::ios::binary);
    if (this->_ofs.is_open())
    {
        this->_ifs.open(DAT_FILE, std::ios::binary);
        if (!this->_ifs.is_open())
            assert(0);

        this->_ifs.tie( &(this->_ofs) );

        this->_ifs >> this->_balance;
        if (this->_ifs.bad())
        {
            this->reset();
            this->sync();
        }
    }
}

SimpleGame::PlayerWallet::~PlayerWallet()
{
    this->_ofs.close();
    this->_ifs.close();
}

void SimpleGame::PlayerWallet::sync()
{
    *(this->_ifs.tie()) << this->_balance;
    if (this->_ofs.bad())
        assert(0);
}

void SimpleGame::PlayerWallet::reset()
{
    this->_balance = DEFAULT_BALANCE;
}

void SimpleGame::PlayerWallet::Deposit(UINT64 ammount)
{
    this->_balance += ammount;
    if (this->_balance > MAX_BALANCE)
        this->_balance = MAX_BALANCE;
}

void SimpleGame::PlayerWallet::Withdraw(UINT64 ammount)
{
    this->_balance -= ammount;
    if (this->_balance < 0)
        this->_balance = 0;
}

/*
 * BetManager implementation
 */

void SimpleGame::BetManager::RaiseWager(UINT64 ammount)
{
    SimpleGame::BetManager::_wager += ammount;
    if (SimpleGame::BetManager::_wager > MAX_WAGER)
        SimpleGame::BetManager::_wager = MAX_WAGER;
}

void SimpleGame::BetManager::LowerWager(UINT64 ammount)
{
    SimpleGame::BetManager::_wager -= ammount;
    if (SimpleGame::BetManager::_wager < 0)
        SimpleGame::BetManager::_wager = 0;
}

UINT64 SimpleGame::BetManager::GetCurrentWager()
{
    return SimpleGame::BetManager::_wager;
}

/*
 * init static members
 */

UINT64 SimpleGame::BetManager::_wager = 0;
SimpleGame::PlayerWallet SimpleGame::BetManager::_myWallet = SimpleGame::PlayerWallet();
