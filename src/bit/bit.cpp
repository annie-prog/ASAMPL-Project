#include "Bit.h"
#include <chrono>
enum class ROTATION
{
    R90 = 1,
    R180,
    R270
};

int Bit::binaryToDecimal(const std::string &binaryNum)
{
    int decimal = stoi(binaryNum, 0, 2);
    return decimal;
}

int Bit::convertToMachineLevel(int n1, int n2)
{
    n2 <<= 8;
    return n2 + n1;
}

void Bit::printMachineLevelCords()
{
    std::vector<int> mlCords = hardWareComp.getMachineLevelCords();
    for (const auto &cord : mlCords)
    {
        std::cout << cord << " ";
    }
    std::cout << std::endl;
}

void Bit::cutBoard()
{
    std::vector<ElectricComponent> &elComps = hardWareComp.getElComps();
    int mCordsBoard = convertToMachineLevel(hardWareComp.getWidth(), hardWareComp.getHeight());
    hardWareComp.setMachineLevelCordsBoard(mCordsBoard);
    for (auto &elComp : elComps)
    {
        elComp.setM_ogPosX(elComp.getM_posX());
        elComp.setM_ogPosY(elComp.getM_posY());
        this->rotateElComp(elComp);
    }

    std::cout << "---------Cut board:" << std::endl;
    std::cout << hardWareComp.getId() << " " << hardWareComp.getMachineLevelCordsBoard() << '\n'
              << '\n';
    hardWareComp.printBoard();

    std::this_thread::sleep_for(std::chrono::seconds(TASK_TIME));
}

void Bit::componentMask()
{
    std::cout << "---------COMPONENTS MASK: " << std::endl;
    std::cout << hardWareComp.getId() << " " << hardWareComp.getMachineLevelCordsBoard() << '\n'
              << '\n';
    printMachineLevelCords();

    std::vector<ElectricComponent> elComps = hardWareComp.getElComps();
    for (auto &elComp : elComps)
    {
        for (int i = 0; i < elComp.getHeight(); ++i)
        {
            for (int j = 0; j < elComp.getWidth(); ++j)
            {
                hardWareComp.getMatrix()[elComp.getM_posY() + i][elComp.getM_posX() + j] = '1';
            }
        }
    }

    hardWareComp.printBoard();

    convertEachLineToBinaryAndPrint();

    std::this_thread::sleep_for(std::chrono::seconds(TASK_TIME));
}
void Bit::printElComps()
{
    std::cout << "---------COMPONENTS: " << '\n'
              << std::endl;
    hardWareComp.machineLevelFormat(std::cout);
    hardWareComp.printBoard();
    std::cout << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(TASK_TIME));
}
void Bit::finalResult()
{
    std::cout << "---------FINAL RESULT: " << '\n'
              << std::endl;
    hardWareComp.descriptionFormat(std::cout);
    hardWareComp.machineLevelFormat(std::cout);
    hardWareComp.visualFormat(std::cout);

    std::this_thread::sleep_for(std::chrono::seconds(TASK_TIME));
}
void Bit::init(const std::string &production, int taskTime)
{
    /*checks if there is an invalid id from printJobs
    (no need for in v1, but it makes testing easier)*/
    if (hardWareComp.getHeight() <= 0)
    {
        return;
    }

    TASK_TIME = taskTime;

    cutBoard();
    showConnections();
    componentMask();
    putElComp();
    putPinsOnBoard();
    printElComps();
    finalResult();
    hardWareComp.initSerialization(production);
}
void Bit::showConnections()
{
    int connectionIdx = 0;
    for (auto &connection : hardWareComp.getPinConnections())
    {
        std::cout << "-----Connection: " << connectionIdx << std::endl;
        std::cout << hardWareComp.getId() << " " << hardWareComp.getMachineLevelCordsBoard() << '\n'
                  << '\n';

        Pin pinA = hardWareComp.getElComps()[connection.getAIdx() - 1].getPins()[connection.getAPinIdx() - 1]; //Gets the propoer pin
        Pin pinB = hardWareComp.getElComps()[connection.getBIdx() - 1].getPins()[connection.getBPinIdx() - 1];

        //Depending on the index of the pin it changes the element of the board accordingly
        hardWareComp.getMatrix()[pinA.getPinY()][pinA.getPinX()] = static_cast<char>(connection.getAPinIdx() + '0');
        hardWareComp.getMatrix()[pinB.getPinY()][pinB.getPinX()] = static_cast<char>(connection.getBPinIdx() + '0');

        //converts the cords of the pin to machine-level
        int currCord = convertToMachineLevel(pinA.getPinX(), pinA.getPinY());
        int currCord2 = convertToMachineLevel(pinB.getPinX(), pinB.getPinY());
        hardWareComp.getMachineLevelCords().push_back(currCord);
        hardWareComp.getMachineLevelCords().push_back(currCord2);
        this->printMachineLevelCords();

        hardWareComp.printBoard();
        //set the board to it's initial state
        hardWareComp.getMatrix()[pinA.getPinY()][pinA.getPinX()] = "*";
        hardWareComp.getMatrix()[pinB.getPinY()][pinB.getPinX()] = "*";

        std::this_thread::sleep_for(std::chrono::seconds(TASK_TIME));

        connectionIdx++;
    }
}

void Bit::pinPosOnBoard(Pin &pin, ElectricComponent &elComp)
{
    int pY = pin.getPinY();
    pY += elComp.getM_posY();

    int pX = pin.getPinX();
    pX += elComp.getM_posX();

    pin.setPinY(pY);
    pin.setPinX(pX);
}

void Bit::putElComp()
{
    std::vector<ElectricComponent> elComps = hardWareComp.getElComps();

    for (auto &elComp : elComps)
    {
        for (int i = 0; i < elComp.getHeight(); ++i)
        {
            for (int j = 0; j < elComp.getWidth(); ++j)
            {
                hardWareComp.getMatrix()[elComp.getM_posY() + i][elComp.getM_posX() + j] = '0';
            }
        }
    }
}

void Bit::putPinsOnBoard()
{
    auto &elComps = hardWareComp.getElComps();
    for (auto &elComp : elComps)
    {
        int pinIdx = 1;
        auto pinVec = elComp.getPins();
        for (auto &pin : pinVec)
        {

            hardWareComp.getMatrix()[pin.getPinY()][pin.getPinX()] = static_cast<char>(pinIdx + '0');
            pinIdx++;
        }
    }
}

void Bit::convertEachLineToBinaryAndPrint()
{
    for (int i = 0; i < hardWareComp.getMatrix().size(); ++i)
    {
        int bitCounter = 0;
        std::string currNumBinary;
        std::vector<int> decimalNums;
        for (int j = 0; j < hardWareComp.getMatrix()[i].size(); ++j)
        {
            if (bitCounter == 8)
            {
                bitCounter = 0;
                decimalNums.push_back(binaryToDecimal(currNumBinary));
                currNumBinary = "";
            }
            if (hardWareComp.getMatrix()[i][j] == "1")
            {
                currNumBinary += "1";
                bitCounter++;
            }
            else
            {
                currNumBinary += "0";
                bitCounter++;
            }
        }
        int k = currNumBinary.size();
        for (; k < EIGHT_BIT; ++k)
        {
            currNumBinary += "0";
        }

        decimalNums.push_back(binaryToDecimal(currNumBinary));

        for (const auto &n : decimalNums)
        {
            std::cout << n << " ";
        }
        std::cout << std::endl;
    }
}

void Bit::rotateElComp(ElectricComponent &elComp)
{
    if (elComp.getRot() == static_cast<int>(ROTATION::R90))
    {
        auto &elCompPins = elComp.getPins();
        for (auto &pin : elCompPins)
        {
            int tempX = pin.getPinY();
            pin.setPinY(pin.getPinX());
            int pY = (elComp.getHeight() - 1) - tempX;
            pin.setPinX(pY);
        }

        int pX = elComp.getM_posX();
        int pH = (elComp.getHeight() - 1);
        pX -= pH;
        elComp.setM_posX(pX);

        int tempX = elComp.getHeight();
        elComp.setHeight(elComp.getWidth());
        elComp.setWidth(tempX);
    }

    else if (elComp.getRot() == static_cast<int>(ROTATION::R180))
    {
        auto &pinVec = elComp.getPins();
        for (auto &pin : pinVec)
        {
            int tempX = pin.getPinX();
            pin.setPinX((elComp.getWidth() - 1) - tempX);
        }

        int pX = elComp.getM_posX();
        int elcompW = elComp.getWidth();
        pX -= elcompW - 1;
        elComp.setM_posX(pX);
    }
    else if (elComp.getRot() == static_cast<int>(ROTATION::R270))
    {
        elComp.setM_posY(elComp.getM_posY() - elComp.getWidth() + 1);
        elComp.setM_posX(elComp.getM_posX() - elComp.getHeight() + 1);

        int ogWidth = elComp.getWidth();
        int ogHeight = elComp.getHeight();

        elComp.setHeight(ogWidth);
        elComp.setWidth(ogHeight);

        auto &pinVec = elComp.getPins();
        for (auto &pin : pinVec)
        {
            int tempX = pin.getPinX();
            int tempY = pin.getPinY();
            pin.setPinY((ogWidth - 1) - tempX);
            pin.setPinX((ogHeight - 1) - tempY);
        }
    }

    auto &pinVec = elComp.getPins();
    for (auto &pin : pinVec)
    {
        this->pinPosOnBoard(pin, elComp);
    }
}
