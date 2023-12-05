#ifndef PIN_H
#define PIN_H
#include <iostream>

class Pin
{
  public:
    Pin(int x, int y);

    int getPinX();
    int getPinY();

    void setPinX(int val);
    void setPinY(int val);

  private:
    int m_pinX;
    int m_pinY;
};
#endif