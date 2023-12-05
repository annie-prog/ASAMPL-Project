#include "OrderManagement.hpp"
#include "TextFileUtils.hpp"
#include <chrono>
#include <ctime>
#include <thread>

//"../../txtFiles/orders.txt" 
//TODO: Modifying some functionality
/*
1. we shouldnt add equal electric and hardware components
2. we should modify adding hardware and electric components not to take ids of the orders
3. we should modify adding hew ids in inventory file
4. we should modify removing all printing orders
5. we should modify the demo for all the functionality to not work with commands
*/

int main()
{
    OrderManagement orders;
    std::string command;
    std::vector<std::string> tokens;

    while (true)
    {
        std::cout << "Enter a command (open/close/save/saveas/check/availableOrders/transfer/exit): ";
        std::getline(std::cin, command);

        tokens.clear();
        size_t startPos = 0;
        size_t endPos = command.find(' ');

        while (endPos != std::string::npos)
        {
            tokens.push_back(command.substr(startPos, endPos - startPos));
            startPos = endPos + 1;
            endPos = command.find(' ', startPos);
        }
        tokens.push_back(command.substr(startPos));

        if (tokens.empty())
        {
            std::cout << "Invalid command. Please try again." << std::endl;
            continue;
        }
        std::string commandType = tokens[0];

        if (commandType == "open")
        {
            ТеxtFileUtils::ProcessOpenCommand(tokens);
        }
        else if (commandType == "close")
        {
            ТеxtFileUtils::ProcessCloseCommand();
        }
        else if (commandType == "save")
        {
            ТеxtFileUtils::ProcessSaveCommand();
        }
        else if (commandType == "saveas")
        {
            ТеxtFileUtils::ProcessSaveAsCommand(tokens);
        }
        else if (commandType == "check")
        {
            std::vector<std::string> orderIds = orders.GetOrderIds("../../txtFiles/orders.txt");
            for (int i = 0; i < orderIds.size(); i++)
            {
                orders.CheckIfTheOrderIsStartedOrNot(orderIds[i]);
            }
        }
        else if (commandType == "availableOrders")
        {
            orders.AddElectricComponent("../../library/electricCompsData.txt");
            orders.AddHardwareComponent("../../library/hardWareCompsData.txt");
            orders.CheckResourceAvailability();
        }
        else if (commandType == "exit")
        {
            ТеxtFileUtils::ProcessExitCommand();
        }
        else if (commandType == "transfer")
        {
            orders.TransferOrderToHistory();
        }
        else if (commandType == "orderComponents")
        {
            std::vector<std::vector<std::string>> orderComponents = orders.SplitComponentsFromOrders();
            for (const auto &components : orderComponents)
            {
                for (const auto &component : components)
                {
                    std::cout << component << std::endl;
                }
            }
        }
        else if (commandType == "splitOrders")
        {
            std::vector<std::string> fileParts = orders.SplitOrders();
            for (const auto &part : fileParts)
            {
                std::cout << part << std::endl;
            }
        }
        else if (commandType == "addElectricComponent")
        {
            orders.AddElectricComponent("../../library/electricCompsData.txt");
        }
        else if (commandType == "addHardwareComponent")
        {
            orders.AddHardwareComponent("../../library/hardWareCompsData.txt");
        }
        else if (commandType == "addNewIds")
        {
            orders.AddNewComponentIntoInventory();
        }
        else if (commandType == "sortOrders")
        {
            orders.GroupOrdersByPriorityAndStatus();
        }
        else if (commandType == "getComponentCountFromOrders")
        {
            std::vector<std::map<std::string, int>> result = orders.GetComponentCountsPerOrder();
            for (const auto &componentCounts : result)
            {
                for (const auto &componentCount : componentCounts)
                {
                    std::cout << componentCount.first << " " << componentCount.second << std::endl;
                }
                std::cout << std::endl;
            }
        }
        else
        {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }
    return 0;
}