#include "menu.hpp"
#include <iostream>

void PrintMenu()
{
    std::cout << "Menu:" << '\n'
              << "1. Print inventory" << '\n'
              << "2. Add missing items to configuration file" << '\n'
              << "3. Check for inventory availability and add quantity" << '\n'
              << "4. Setting the minimum quantities for different components" << '\n'
              << "0. Exit" << '\n';
}

void RunMenu(AssetMonitor &amp, const std::string &inventoryPath)
{
    int choice;
    do
    {
        PrintMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::string id;
        int quantity = 0;
        switch (choice)
        {
        case 1:
            amp.PrintInventory();
            break;
        case 2:
            amp.AddMissingItemsToConfig();
            break;
        case 3:
            amp.CheckInventory();
            break;
        case 4:
            do
            {
                std::cout << "Enter id and quantity : ";
                std::cin >> id >> quantity;
            } while (quantity < 0);
            amp.SetMinimumQuantity(id, quantity);
            break;
        case 0:
            std::cout << "Exit" << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}

void run(const std::string &configPath, const std::string &inventoryPath)
{
    try
    {
        AssetMonitor amp(configPath, inventoryPath);
        RunMenu(amp, inventoryPath);
    }
    catch (const std::exception &e)
    {
        std::cout << "Exception occurred: " << e.what() << std::endl;
    }
}