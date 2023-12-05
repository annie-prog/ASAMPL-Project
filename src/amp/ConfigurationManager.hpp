#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

class ConfigurationManager
{
  public:
    ConfigurationManager(const std::string &fileName);

    unsigned int GetMinimumInventory(const std::string &id) const;
    void UpdateConfiguration(const std::string &id, int quantity);
    void AddNewItem(const std::string &id, int minimumQuantity);

  private:
    std::unordered_map<std::string, int> configuration;

    const std::string configFile;
};

#endif // CONFIGURATIONMANAGER_H