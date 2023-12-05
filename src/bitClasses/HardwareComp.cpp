#include "HardwareComp.h"
//null everything
HardwareComp::HardwareComp() : machineLevelCords(0), m_width(0), m_height(0)
{
}

HardwareComp::HardwareComp(const HardwareComp &original)
{
    this->m_id = original.m_id;
    this->m_width = original.m_width;
    this->m_height = original.m_height;
    this->machineLevelCords = original.machineLevelCords;
    this->m_elComps = original.m_elComps;
    this->m_connections = original.m_connections;
    this->m_boardMatrix.resize(m_height, std::vector<std::string>(m_width));
    this->fillBoard(this->m_boardMatrix);
}

void HardwareComp::addElComp(ElectricComponent &elComp)
{
    this->m_elComps.push_back(elComp);
}

void HardwareComp::addConnection(PinConnections &con)
{
    this->m_connections.push_back(con);
}

void HardwareComp::fillBoard(Board &m_boardMatrix)
{
    for (int i = 0; i < m_boardMatrix.size(); ++i)
    {
        for (int j = 0; j < m_boardMatrix[i].size(); ++j)
        {
            m_boardMatrix[i][j] = '*';
        }
    }
}

void HardwareComp::printBoard() const
{
    for (int i = 0; i < this->m_boardMatrix.size(); ++i)
    {
        for (int j = 0; j < this->m_boardMatrix[i].size(); ++j)
        {
            std::cout << this->m_boardMatrix[i][j];
        }
        std::cout << std::endl;
    }
}

std::string &HardwareComp::getId()
{
    return this->m_id;
}

int HardwareComp::getMachineLevelCordsBoard()
{
    return this->machineLevelCordsBoard;
}

int HardwareComp::getWidth()
{
    return this->m_width;
}

int HardwareComp::getHeight()
{
    return this->m_height;
}

void HardwareComp::setId(std::string &id)
{
    this->m_id = id;
}

void HardwareComp::setWidth(int w)
{
    this->m_width = w;
}
void HardwareComp::setMachineLevelCordsBoard(int cordsDecimal)
{
    this->machineLevelCordsBoard = cordsDecimal;
}
void HardwareComp::setHeight(int h)
{
    this->m_height = h;
}

std::vector<ElectricComponent> &HardwareComp::getElComps()
{
    return this->m_elComps;
}

std::vector<PinConnections> &HardwareComp::getPinConnections()
{
    return this->m_connections;
}

void HardwareComp::initSerialization(std::string production)
{
    std::fstream myFile;
    //TODO: needs to be error checked(for version 2.0)
    myFile.open("../../" + production, std::ios::out | std::ios::app);
    descriptionFormat(myFile);
    machineLevelFormat(myFile);
    visualFormat(myFile);
    myFile.close();
}

void HardwareComp::descriptionFormat(std::ostream &myFile)
{
    myFile << "Description Format: " << '\n';
    myFile << "id: " << this->m_id << '\n';
    myFile << "width: " << this->m_width << '\n';
    myFile << "heigth: " << this->m_height << '\n';
    myFile << "components:" << '\n';

    for (auto &elComp : this->m_elComps)
    {
        myFile << elComp.getId() << ": "
               << "(" << elComp.getM_ogPosX() << ", " << elComp.getM_ogPosY() << "), " << elComp.getRot() << '\n';
    }

    myFile << "connections:";

    for (int i = 0; i < this->m_connections.size() - 1; ++i)
    {
        this->m_connections[i].printPinsConns(myFile);
        myFile << ',';
    }

    this->m_connections[m_connections.size() - 1].printPinsConns(myFile);
    myFile << '\n'
           << '\n';
}

void HardwareComp::visualFormat(std::ostream &myFile)
{
    myFile << "Visual Format:" << '\n';
    myFile << m_id << '\n'
           << '\n';
    for (int i = 0; i < this->m_boardMatrix.size(); ++i)
    {
        for (int j = 0; j < this->m_boardMatrix[i].size(); ++j)
        {
            myFile << this->m_boardMatrix[i][j];
        }
        myFile << '\n';
    }
    myFile << '\n';
}

void HardwareComp::machineLevelFormat(std::ostream &myFile)
{
    struct ElCompData
    {
        int numOccurances = 0;
        std::vector<int> posX;
        std::vector<int> posY;
        std::vector<int> rot;
    };
    std::map<std::string, ElCompData> numOfEachElComp;
    for (auto &elComp : m_elComps)
    {
        numOfEachElComp[elComp.getId()].numOccurances += 1;
        numOfEachElComp[elComp.getId()].posX.push_back(elComp.getM_ogPosX());
        numOfEachElComp[elComp.getId()].posY.push_back(elComp.getM_ogPosY());
        numOfEachElComp[elComp.getId()].rot.push_back(elComp.getRot());
    }
    myFile << "Machine-Level Format: " << getMachineLevelCordsBoard() << '\n';
    myFile << m_id << ":" << '\n';
    for (const auto &kv : numOfEachElComp)
    {
        myFile << "    " << kv.first << ": ";
        myFile << kv.second.numOccurances << " ";
        for (int i = 0; i < kv.second.posX.size(); ++i)
        {
            myFile << '(' << kv.second.posX[i] << "," << kv.second.posY[i] << ") " << kv.second.rot[i] << " ";
        }
        myFile << '\n';
    }
    for (const auto &cord : this->machineLevelCords)
    {
        myFile << cord << " ";
    }
    myFile << '\n'
           << '\n';
}

Board &HardwareComp::getMatrix()
{
    return this->m_boardMatrix;
}

std::vector<int> &HardwareComp::getMachineLevelCords()
{
    return this->machineLevelCords;
}
