#include "AssetMonitor.hpp"

AssetMonitor::AssetMonitor(const std::string &configPath, const std::string &inventoryPath)
    : m_config(configPath), m_inventory(inventoryPath)
{
}

void AssetMonitor::PrintInventory()
{
    m_inventory.PrintInventory();
}

void AssetMonitor::AddMissingItemsToConfig()
{
    const int DEFAULT_QUANTITY = 5;
    std::vector<std::string> IDs = m_inventory.GetInventory();
    for (const auto &id : IDs)
    {
        try
        {
            m_config.GetMinimumInventory(id);
        }
        catch (const std::out_of_range &e)
        {
            m_config.AddNewItem(id, DEFAULT_QUANTITY);
        }
    }
}

int AssetMonitor::CheckInventory(const std::string &id)
{
    int quantity = 0;
    int minimumQuantity = 0;
    bool result = false;
    minimumQuantity = m_config.GetMinimumInventory(id);
    try
    {
        quantity = m_inventory.GetQuantity(id);
    }
    catch (const std::logic_error &e)
    {
        std::cerr << e.what();
    }

    if (quantity < minimumQuantity)
    {
        try
        {
            m_inventory.Order(id, minimumQuantity - quantity);
            quantity = minimumQuantity;
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what();
        }
    }
    return quantity;
}

void AssetMonitor::CheckInventory()
{
    for (auto &product : m_inventory.GetInventory())
    {
        CheckInventory(product);
    }
}

void AssetMonitor::Order(const std::string &id, int quantity)
{
    m_inventory.Order(id, quantity);
}

void AssetMonitor::SetMinimumQuantity(const std::string &id, int quantity)
{
    try
    {
        m_config.UpdateConfiguration(id, quantity);
    }
    catch (std::runtime_error)
    {
    }
    catch (std::logic_error)
    {
    }
}