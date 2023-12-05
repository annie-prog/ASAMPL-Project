#include "pin.h"

Pin::Pin(int x, int y)
    : m_pinX(x), m_pinY(y)
{
}

int Pin::getPinX()
{
    return this->m_pinX;
}

int Pin::getPinY()
{
    return this->m_pinY;
}

void Pin::setPinX(int val)
{
    this->m_pinX = val;
}

void Pin::setPinY(int val)
{
    this->m_pinY = val;
}
