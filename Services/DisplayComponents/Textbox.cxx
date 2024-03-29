#define ADD_EXPORTS

#include <Textbox.hxx>
#include <cstring>

GameService::Textbox::Textbox() : _wText(NULL), _textLen(0), _pRect(NULL), _finalized(false)
{

}

GameService::Textbox::Textbox(char *text, Vector2 position, Vector2 boxSize)
{
    this->_position = position;
    this->_wText = charToWChar(text);
    this->_textLen = strlen(text);
    this->_boxSize = boxSize;
    this->_downRight = Vector2(position.GetX() + boxSize.GetX(), position.GetY() + boxSize.GetY());

    this->_pRect = (LPRECT) malloc(sizeof(tagRECT));
    this->_pRect->left = this->_position.GetX();
    this->_pRect->top = this->_position.GetY();
    this->_pRect->right = this->_downRight.GetX();
    this->_pRect->bottom = this->_downRight.GetY();

    this->_finalized = false;
}

void GameService::Textbox::Draw(HDC targetHDC)
{
    SetTextColor(targetHDC, Color::rgbGreen);
    SetBkColor(targetHDC, Color::rgbBlack);
    //SetBkMode(targetHDC, TRANSPARENT);
    DrawTextW(targetHDC, this->_wText, this->_textLen, this->_pRect, DT_BOTTOM);
}

GameService::Vector2 GameService::Textbox::GetPosition() const
{
    return this->_position;
}

LPRECT GameService::Textbox::getRectPtr() const
{
    return this->_pRect;
}

void GameService::Textbox::SetPosition(const Vector2 &position)
{
    if (position.GetX() >= 0 && position.GetY() >= 0)
        this->_position = position;
}

void GameService::Textbox::SetText(const char *text)
{
    this->_wText = charToWChar(text);
    this->_textLen = strlen(text);
}

void GameService::Textbox::SetSize(const Vector2 &boxSize)
{
    this->_boxSize = boxSize;
    this->_downRight = Vector2(this->_position.GetX() + boxSize.GetX(), this->_position.GetY() + boxSize.GetY());
}

void GameService::Textbox::Finalize()
{
    free((LPVOID)this->_wText);
    free(this->_pRect);

    this->_wText = NULL;
    this->_pRect = NULL;
    this->_finalized = true;
}

GameService::Textbox::~Textbox()
{
    if (!this->_finalized) this->Finalize();
}