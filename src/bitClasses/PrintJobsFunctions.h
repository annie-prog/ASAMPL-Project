#ifndef PRINT_JOBS_FUNCTIONS
#define PRINT_JOBS_FUNCTIONS
#include "hardWareComp.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace pj
{
const std::vector<HardwareComp> getIdFromPj(const std::string &printJobs); //Gets the id of Hardware Component and finds in the library
void buildHardWareCompFromLib(const std::string &id, HardwareComp &h);
void buildElCompFromLib(const std::string &id, HardwareComp &h);
void buildPinAndAddToElComp(std::string &currPin, ElectricComponent &elComp);
void buildPinConnectionsAndAddToHardComp(std::string &connection, HardwareComp &h); //Build and add to HardWare Comp
} // namespace pj
#endif