#include "ConfigurationManager.hpp"

ConfigurationManager::ConfigurationManager(const std::string &fileName)
    : configFile(fileName)
{
    std::ifstream fileHandler(configFile);
    if (!fileHandler.is_open())
    {
        throw std::runtime_error("Config file error begin");
    }
    std::string id;
    int quantity;
    while (fileHandler >> id >> quantity)
    {
        configuration.emplace(id, quantity);
    }
    if (!fileHandler.good() && !fileHandler.eof())
    {
        fileHandler.close();
        throw std::runtime_error("Config file error end");
    }
    fileHandler.close();
}

unsigned int ConfigurationManager::GetMinimumInventory(const std::string &id) const
{
    const auto elComponent = configuration.find(id);
    if (elComponent != configuration.end())
    {
        return elComponent->second;
    }
    throw std::out_of_range("Item not found");
}

void ConfigurationManager::UpdateConfiguration(const std::string &id, int quantity)
{
    auto elComponent = configuration.find(id);
    if (elComponent != configuration.end())
    {
        elComponent->second = quantity;
        std::ofstream fileHandler(configFile, std::ios_base::trunc);
        if (!fileHandler.is_open())
        {
            throw std::runtime_error("File error");
        }
        for (const auto &product : configuration)
        {
            if (!fileHandler.good())
            {
                fileHandler.close();
                throw std::runtime_error("File error");
            }
            fileHandler << product.first << " " << product.second << std::endl;
        }
        fileHandler.close();
    }
    else
    {
        throw std::out_of_range("Item not found");
    }
}

void ConfigurationManager::AddNewItem(const std::string &id, int minimumQuantity)
{
    if (configuration.find(id) == configuration.end())
    {
        configuration.emplace(id, minimumQuantity);

        std::ofstream fileHandler(configFile, std::ios_base::app);
        if (!fileHandler.is_open())
        {
            throw std::runtime_error("File error");
        }
        fileHandler << id << " " << minimumQuantity << std::endl;
        fileHandler.close();
    }
    else
    {
        throw std::logic_error("Item already exist");
    }
}