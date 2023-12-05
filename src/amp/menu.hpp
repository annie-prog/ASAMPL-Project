#ifndef MENU_HPP
#define MENU_HPP

#include "AssetMonitor.hpp"

void PrintMenu();
void RunMenu(AssetMonitor &amp, const std::string &inventoryPath);
void run(const std::string &configPath, const std::string &inventoryPath);

#endif // MENU_HPP