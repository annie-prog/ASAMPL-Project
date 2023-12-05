#ifndef ELECTRIC_COMPONENT_H
#define ELECTRIC_COMPONENT_H
#include "fstream"
#include "pin.h"
#include <iostream>
#include <vector>

class ElectricComponent
{

  public:
    ElectricComponent();

    void addPin(Pin &currentPin);

    int getWidth();
    int getHeight();
    int getM_ogPosX();
    int getM_ogPosY();
    int getM_posX();
    int getM_posY();
    int getRot();
    std::string &getId();
    std::vector<Pin> &getPins();

    void setWidth(int w);
    void setHeight(int h);
    void setM_ogPosX(int pos);
    void setM_ogPosY(int pos);
    void setM_posX(int pos);
    void setM_posY(int pos);
    void setRot(int rot);
    void setId(std::string &id);

    void printComp(); //Prints comp id: posX, posY rotation for description format

  private:
    std::string m_ElCompId;
    int m_ElCompWidth;
    int m_ElCompHeight;

    int m_posX;
    int m_posY;

    int m_ogPosX;
    int m_ogPosY;

    int m_rotation;
    std::vector<Pin> pins;
};

#endif