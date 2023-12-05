#ifndef ASSETMONITOR_H
#define ASSETMONITOR_H

#include "ConfigurationManager.hpp"
#include "InventoryManager.hpp"
#include <stdexcept>
#include <string>

class AssetMonitor
{
  public:
    AssetMonitor(const std::string &configPath, const std::string &inventoryPath);

    void PrintInventory();

    void AddMissingItemsToConfig();

    int CheckInventory(const std::string &id);

    void CheckInventory();
    void Order(const std::string &id, int quantity);
    void SetMinimumQuantity(const std::string &id, int quantity);

  private:
    ConfigurationManager m_config;
    InventoryManager m_inventory;
};

#endif // ASSETMONITOR_H