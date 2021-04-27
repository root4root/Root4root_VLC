#ifndef _LIB_ROOT4ROOT_VLC_
#define _LIB_ROOT4ROOT_VLC_

#include "Arduino.h"

template <typename T>
class Root4root_VLC
{
public:
    Root4root_VLC(T *lcdHandler, uint8_t chars, uint8_t lines, uint8_t offsetX, uint8_t offsetY);
   ~Root4root_VLC();

    void setLine(uint8_t line, bool resetX = true);
    void setCursor(uint8_t x = 0, uint8_t y = 0);

    void setDelayTime(uint16_t delay);
    void resetDelayCounter();

    void displayString(const char *str, bool moveCursor = true);
    void write(uint8_t symbol);
    void clear();

private:
    char *lcdState = NULL;

    uint8_t lcdLines{};         //number of lcd lines
    uint8_t lcdChars{};         //number of lcd chars in line

    uint8_t offsetX{};          //Top-left corner of Virtual LCD
    uint8_t offsetY{};          //"-"

    uint8_t internalCursorX{};
    uint8_t internalCursorY{};

    uint32_t previuosChangeTime{};
    uint16_t displayDelay{};

    T *lcdHandler{}; //NULL PTR
};

template <typename T>
Root4root_VLC<T>::Root4root_VLC(T *lcd, uint8_t chars, uint8_t lines, uint8_t offsetX, uint8_t offsetY):
lcdHandler(lcd), lcdLines(lines), lcdChars(chars), offsetX(offsetX), offsetY(offsetY)
{
    lcdState = new char[lines * chars + 1]();
}

template <typename T>
void Root4root_VLC<T>::setDelayTime(uint16_t delay) //Starting from 0
{
    displayDelay = delay;
}

template <typename T>
void Root4root_VLC<T>::resetDelayCounter()
{
    previuosChangeTime = 0;
}


template <typename T>
void Root4root_VLC<T>::setLine(uint8_t line, bool resetX) //Starting from 0
{
    if (line > lcdLines) {
        return;
    }

    if (resetX) {
        internalCursorX = 0;
    }

    internalCursorY = line;

    lcdHandler->setCursor(offsetX + internalCursorX, offsetY + internalCursorY);
}

template <typename T>
void Root4root_VLC<T>::setCursor(uint8_t x, uint8_t y) //Internal function
{
    if (x < lcdChars && y < lcdLines ) {
        this->internalCursorX = x;
        this->internalCursorY = y;
    }

    lcdHandler->setCursor(offsetX + internalCursorX, offsetY + internalCursorY);
}

template <typename T>
void Root4root_VLC<T>::displayString(const char *str, bool moveCursor)
{
    if (displayDelay > 0 && (millis() - previuosChangeTime) < displayDelay) {
        return;
    }

    if (internalCursorX >= lcdChars) { return; }

    previuosChangeTime = millis();

    uint8_t start = internalCursorY * lcdChars + internalCursorX;
    uint8_t head = 0; //Gap sensor to determine if we need setCursor command.

    if (str[0] != lcdState[start]) {
        lcdHandler->setCursor(offsetX + internalCursorX, offsetY + internalCursorY);
    }

    uint8_t i = 0;

    for ( ; i < lcdChars - internalCursorX; ++i) {

        if (str[i] == '\0') {
            break;
        }

        if (str[i] != lcdState[start + i]) {
            lcdState[start + i] = str[i];
            if (head != i) {
                lcdHandler->setCursor(offsetX + internalCursorX + i, offsetY + internalCursorY);
                head = i;
            }
            lcdHandler->write(str[i]);
            ++head;
        }
    }

    if (moveCursor) {
        internalCursorX += i;
    }
}

template <typename T>
void Root4root_VLC<T>::write(uint8_t symbol)
{
    lcdHandler->write(symbol);
}

template <typename T>
void Root4root_VLC<T>::clear()
{
    char container[lcdChars + 1] = {};
    uint8_t i = 0;

    for (i = 0; i < lcdChars; ++i) {
        container[i] = ' ';
    }

    for (i = 0; i < lcdLines; ++i) {
        setLine(i);

        displayString(container, false);
    }

    internalCursorX = 0;
    internalCursorY = 0;

    memset(lcdState, 0, lcdLines * lcdChars + 1);
}

template <typename T>
Root4root_VLC<T>::~Root4root_VLC()
{
    delete [] lcdState;
}

#endif //_LIB_ROOT4ROOT_VLC_

