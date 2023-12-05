#include "PinConnections.h"

PinConnections::PinConnections(int compA, int compAPin, int compB, int compBPin) : m_compAIdx(compA),
                                                                                   m_compAPinIdx(compAPin),
                                                                                   m_compBIdx(compB),
                                                                                   m_compBPinIdx(compBPin)
{
}

void PinConnections::printPinsConns(std::ostream &myFile)
{
    myFile << "(" << m_compAIdx << "." << m_compAPinIdx << ", " << m_compBIdx << "." << m_compBPinIdx << ")";
}

int PinConnections::getAIdx()
{
    return this->m_compAIdx;
}

int PinConnections::getAPinIdx()
{
    return this->m_compAPinIdx;
}
int PinConnections::getBIdx()
{
    return this->m_compBIdx;
}

int PinConnections::getBPinIdx()
{
    return this->m_compBPinIdx;
}
