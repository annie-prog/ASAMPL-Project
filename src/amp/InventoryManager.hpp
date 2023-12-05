#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

class InventoryManager
{
  public:
    InventoryManager(const std::string &fileName);

    void Order(const std::string &id, int quantity);
    int GetQuantity(const std::string &id) const;
    std::vector<std::string> GetInventory() const;
    void PrintInventory() const;

  private:
    std::unordered_map<std::string, int> inventory;
    const std::string inventoryFile;
};

#endif // INVENTORYMANAGER_H