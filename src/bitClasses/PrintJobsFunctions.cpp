#include "PrintJobsFunctions.h"

const std::vector<HardwareComp> pj::getIdFromPj(const std::string &printJobs)
{

    std::vector<HardwareComp> hardWareComps;
    std::fstream myFile;
    myFile.open("../../" + printJobs, std::ios::in);
    std::string line;
    std::string id;
    while (std::getline(myFile, line))
    {
        if (line.find("name") != std::string::npos)
        {
            HardwareComp hardWareComp;
            std::size_t pos = line.find(':');
            id = line.substr(pos + 2); // +2 in order to skip the " "
            buildHardWareCompFromLib(id, hardWareComp);
            hardWareComps.push_back(hardWareComp);
        }
    }
    myFile.close();
    return hardWareComps;
}
void pj::buildHardWareCompFromLib(const std::string &id, HardwareComp &h)
{
    struct ElCompPosRot
    {
        std::string m_posX;
        std::string m_posY;
        std::string m_rot;
    };
    std::fstream myFile;
    myFile.open("../../library/hardWareCompsData.txt", std::ios::in);
    std::string line;
    while (std::getline(myFile, line))
    {
        std::stringstream ss(line);
        std::string currId;
        std::getline(ss, currId, ';');
        if (currId != id)
        {
            continue;
        }
        h.setId(currId);
        std::string width, height;
        std::getline(ss, width, ';');
        h.setWidth(std::stoi(width));
        std::getline(ss, height, ';');
        h.setHeight(std::stoi(height));
        std::string elCompsStr;
        std::getline(ss, elCompsStr, ';');
        std::string elComp;
        std::stringstream elCompsStream(elCompsStr);
        std::vector<ElCompPosRot> posRots;
        while (std::getline(elCompsStream, elComp, '|'))
        {
            std::stringstream elCompStream(elComp);
            std::string elCompId;
            ElCompPosRot elCompPosRot;
            std::getline(elCompStream, elCompId, ':');
            std::getline(elCompStream, elCompPosRot.m_posX, ',');
            std::getline(elCompStream, elCompPosRot.m_posY, ',');
            std::getline(elCompStream, elCompPosRot.m_rot, ',');
            posRots.push_back(elCompPosRot);
            buildElCompFromLib(elCompId, h);
        }
        for (int i = 0; i < h.getElComps().size(); ++i)
        {
            int posX = std::stoi(posRots[i].m_posX);
            int posY = std::stoi(posRots[i].m_posY);
            int rot = std::stoi(posRots[i].m_rot);
            h.getElComps()[i].setM_posX(posX);
            h.getElComps()[i].setM_posY(posY);
            h.getElComps()[i].setRot(rot);
        }
        std::string connections;
        std::getline(ss, connections, ';');
        std::stringstream connsStream(connections);
        std::string connection;
        while (std::getline(connsStream, connection, '|'))
        {
            buildPinConnectionsAndAddToHardComp(connection, h);
        }
    }
    myFile.close();
}

void pj::buildElCompFromLib(const std::string &id, HardwareComp &h)
{
    ElectricComponent elComp;
    std::fstream myFile;
    myFile.open("../../library/electricCompsData.txt", std::ios::in);
    std::string line;

    while (std::getline(myFile, line))
    {
        std::stringstream ss(line);
        std::string currId;
        std::getline(ss, currId, ';');
        if (currId != id)
        {
            continue;
        }
        std::string width, height, pinsString;
        elComp.setId(currId);
        std::getline(ss, width, ';');
        std::getline(ss, height, ';');
        std::getline(ss, pinsString, ';');
        elComp.setWidth(std::stoi(width));
        elComp.setHeight(std::stoi(height));
        std::stringstream pinsStream(pinsString);
        std::string currPin;
        while (std::getline(pinsStream, currPin, '|'))
        {
            buildPinAndAddToElComp(currPin, elComp);
        }
        h.addElComp(elComp);
    }
    myFile.close();
}

void pj::buildPinAndAddToElComp(std::string &currPin, ElectricComponent &elComp)
{
    std::stringstream currPinStream(currPin);
    std::string x, y;
    std::getline(currPinStream, x, ',');
    std::getline(currPinStream, y, ',');
    Pin p(std::stoi(x), std::stoi(y));
    elComp.addPin(p);
}

void pj::buildPinConnectionsAndAddToHardComp(std::string &connection, HardwareComp &h)
{
    std::stringstream connStream(connection);
    std::string compAIdx, compAPinIdx, compBIdx, compBPinIdx;
    std::getline(connStream, compAIdx, '.');
    std::getline(connStream, compAPinIdx, ',');
    std::getline(connStream, compBIdx, '.');
    std::getline(connStream, compBPinIdx);
    PinConnections currConnection(std::stoi(compAIdx), std::stoi(compAPinIdx), std::stoi(compBIdx), std::stoi(compBPinIdx));
    h.getPinConnections().push_back(currConnection);
}
