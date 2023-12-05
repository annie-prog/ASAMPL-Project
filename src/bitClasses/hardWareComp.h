#ifndef HARDWARE_COMP_H
#define HARDWARE_COMP_H
#include "ElectricComp.h"
#include "PinConnections.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using Board = std::vector<std::vector<std::string>>;

class HardwareComp
{
  public:
    HardwareComp();
    HardwareComp(const HardwareComp &original);

    void addElComp(ElectricComponent &elComp);
    void addConnection(PinConnections &con);

    std::string &getId();
    int getMachineLevelCordsBoard();
    int getWidth();
    int getHeight();
    Board &getMatrix();
    std::vector<int> &getMachineLevelCords();
    std::vector<ElectricComponent> &getElComps();
    std::vector<PinConnections> &getPinConnections();

    void setMachineLevelCordsBoard(int cordsDecimal);
    void setId(std::string &id);
    void setWidth(int w);
    void setHeight(int h);

    void printBoard() const;

    void initSerialization(std::string production); // Opens myFile and closes it after the functions below are done
    //Functions below print to production.txt
    void descriptionFormat(std::ostream &myFile);
    void machineLevelFormat(std::ostream &myFile);
    void visualFormat(std::ostream &myFile);

  private:
    int machineLevelCordsBoard;
    std::string m_id;
    int m_width;
    int m_height;
    Board m_boardMatrix;
    std::vector<int> machineLevelCords;
    std::vector<ElectricComponent> m_elComps;
    std::vector<PinConnections> m_connections;

    void fillBoard(Board &m_boardMatrix); // fills the board initially with *
};
#endif