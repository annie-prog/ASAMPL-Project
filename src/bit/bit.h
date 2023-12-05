#ifndef BIT_H
#define BIT_H
#include "../bitClasses/hardwareComp.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#define EIGHT_BIT 8
class Bit
{
  public:
    //TODO: change to move constructor(might be bugs, but it is a good optimization if it works)
    Bit(const HardwareComp &original) : hardWareComp(original) {}
    void init(const std::string &production, int taskTime); // Starts the building proccess

  private:
    HardwareComp hardWareComp;

    int TASK_TIME = 0;

    //TODO: change to unsigned char
    int binaryToDecimal(const std::string &binaryNum);
    int convertToMachineLevel(int n1, int n2);

    void rotateElComp(ElectricComponent &elComp);
    void pinPosOnBoard(Pin &pin, ElectricComponent &elComp); // Converts pin cords to the cords they are gonna be on the board
    void putPinsOnBoard();
    void putElComp();

    void convertEachLineToBinaryAndPrint();

    void cutBoard();
    void printMachineLevelCords();
    void showConnections();
    void componentMask();
    void printElComps();
    void finalResult();
};
#endif