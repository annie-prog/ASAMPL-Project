#ifndef PIN_CONNECTIONS_H
#define PIN_CONNECTIONS_H
#include <fstream>
#include <iostream>
class PinConnections
{
  public:
    PinConnections(int compA, int compAPin, int compB, int compBPin);

    void printPinsConns(std::ostream &myFile); // Prints pin connections to production.txt

    int getAIdx();
    int getAPinIdx();
    int getBIdx();
    int getBPinIdx();

  private:
    int m_compAIdx;
    int m_compAPinIdx;
    int m_compBIdx;
    int m_compBPinIdx;
};
#endif