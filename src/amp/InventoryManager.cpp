#include "InventoryManager.hpp"

InventoryManager::InventoryManager(const std::string &fileName)
    : inventoryFile(fileName)
{
    std::ifstream fileHandler(inventoryFile);
    if (!fileHandler.is_open())
    {
        throw std::runtime_error("Inventory file error begin");
    }
    std::string id;
    int quantity;
    while (fileHandler >> id >> quantity)
    {
        inventory.emplace(id, quantity);
    }
    if (!fileHandler.good() && !fileHandler.eof())
    {
        fileHandler.close();
        throw std::runtime_error("Inventory file error begin");
    }
    fileHandler.close();
}

void InventoryManager::Order(const std::string &id, int quantity)
{
    bool isFound = false;
    auto elComponent = inventory.find(id);
    if (elComponent != inventory.end())
    {
        elComponent->second += quantity;
        isFound = true;
    }

    if (!isFound)
    {
        inventory.emplace(id, quantity);
    }

    std::ofstream fileHandler(inventoryFile, std::ios_base::trunc);
    if (!fileHandler.is_open())
    {
        throw std::runtime_error("File error");
    }

    for (const auto &product : inventory)
    {
        fileHandler << product.first << " " << product.second << std::endl;
    }
    if (!fileHandler.good())
    {
        fileHandler.close();
        throw std::runtime_error("File error");
    }
    fileHandler.close();
}

int InventoryManager::GetQuantity(const std::string &id) const
{
    auto elComponent = inventory.find(id);
    if (elComponent != inventory.end())
    {
        return elComponent->second;
    }
    else
    {
        throw std::logic_error("Item not found");
    }
}
std::vector<std::string> InventoryManager::GetInventory() const
{
    std::vector<std::string> tmp;
    tmp.reserve(inventory.size());
    for (const auto &item : inventory)
    {
        tmp.push_back(item.first);
    }
    return tmp;
}

void InventoryManager::PrintInventory() const
{
    for (const auto &product : inventory)
    {
        std::cout << product.first << " " << product.second << std::endl;
    }
}
