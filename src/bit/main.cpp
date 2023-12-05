/*
TODO: Changes
1. Error checking and error handling
2. Change Bit: 
    convertToMachineLevel() uses int, need to use unsigned char
3. Change HardwareComp:
    change copy constructor to move constructor 
*/
#include "../bitClasses/PrintJobsFunctions.h"
#include "bit.h"
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    std::string taskTimeStr;

    std::ifstream cfgConfig("cfg_bit.txt");
    cfgConfig >> taskTimeStr;
    cfgConfig.close();

    std::string printJobs = argv[1];
    std::string production = argv[2];

    int taskTime = std::stoi(taskTimeStr);

    auto &hardWareComps = pj::getIdFromPj(printJobs);
    for (auto &h : hardWareComps)
    {
        Bit b(h);
        b.init(production, taskTime);
    }

    return 0;
}