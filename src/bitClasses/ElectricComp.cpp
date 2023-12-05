#include "ElectricComp.h"

ElectricComponent::ElectricComponent() : m_ElCompWidth(0), m_ElCompHeight(0), m_posX(0), m_posY(0), m_ogPosX(0), m_ogPosY(0), m_rotation(0)
{
}

void ElectricComponent::addPin(Pin &currentPin)
{
    this->pins.push_back(currentPin);
}

int ElectricComponent::getWidth() { return this->m_ElCompWidth; }

int ElectricComponent::getHeight() { return this->m_ElCompHeight; }

int ElectricComponent::getM_ogPosX()
{
    return this->m_ogPosX;
}
int ElectricComponent::getM_ogPosY()
{
    return this->m_ogPosY;
}
int ElectricComponent::getM_posX()
{
    return this->m_posX;
}
int ElectricComponent::getM_posY()
{
    return this->m_posY;
}

int ElectricComponent::getRot()
{
    return this->m_rotation;
}

void ElectricComponent::setWidth(int w)
{
    this->m_ElCompWidth = w;
}

void ElectricComponent::setHeight(int h)
{
    this->m_ElCompHeight = h;
}

void ElectricComponent::setM_ogPosX(int pos)
{
    this->m_ogPosX = pos;
}

void ElectricComponent::setM_ogPosY(int pos)
{
    this->m_ogPosY = pos;
}

void ElectricComponent::setM_posX(int pos)
{
    this->m_posX = pos;
}

void ElectricComponent::setM_posY(int pos)
{
    this->m_posY = pos;
}

void ElectricComponent::setRot(int rot)
{
    this->m_rotation = rot;
}

void ElectricComponent::setId(std::string &id)
{
    this->m_ElCompId = id;
}

std::string &ElectricComponent::getId() { return this->m_ElCompId; }

std::vector<Pin> &ElectricComponent::getPins()
{
    return this->pins;
}

void ElectricComponent::printComp()
{
    std::cout << m_ElCompId << ": "
              << "(" << m_ogPosX << ", " << m_ogPosY << "), " << m_rotation << '\n';
}
