#include "OrderManagement.hpp"
#include <algorithm>

OrderManagement::OrderManagement() : m_status(Status::Unprocessed) {}
std::string TrimInFile(const std::string &str)
{
    const std::string whiteSpace = " \t\r\n";
    auto startSymbol = str.find_first_not_of(whiteSpace);
    auto endSymbol = str.find_last_not_of(whiteSpace);
    if (startSymbol == std::string::npos || endSymbol == std::string::npos)
    {
        return "";
    }
    return str.substr(startSymbol, endSymbol - startSymbol + 1);
}
std::vector<std::string> OrderManagement::SplitOrders()
{
    std::ifstream file(ORDERS_FILENAME);

    if (!file)
    {
        std::cerr << "Error opening file: " << ORDERS_FILENAME << std::endl;
        return this->m_hardwareComponents;
    }

    std::string line;
    std::string part;
    while (std::getline(file, line))
    {
        if (!line.empty())
        {
            part += line + "\n";
        }
        else
        {
            if (!part.empty())
            {
                this->m_hardwareComponents.push_back(part);
                part = "";
            }
        }
    }
    if (!part.empty())
    {
        this->m_hardwareComponents.push_back(part);
    }

    file.close();
    return this->m_hardwareComponents;
}
void OrderManagement::CheckIfTheOrderIsStartedOrNot(const std::string &orderId)
{
    std::ifstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cerr << "Problem with opening the file!" << std::endl;
        return;
    }

    std::string line;
    std::string currentOrderId;
    bool orderFound = false;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string idString;
        std::string currentOrderId;

        if (std::getline(iss, idString, ':') && std::getline(iss, currentOrderId))
        {
            if (idString == "id")
            {
                currentOrderId = TrimInFile(currentOrderId);
                if (currentOrderId == orderId)
                {
                    orderFound = true;
                    break;
                }
            }
        }
    }
    if (orderFound)
    {
        if (this->m_status == Status::Unprocessed)
        {
            std::cout << "The order " << orderId << " is not started yet!" << std::endl;
        }
        else
        {
            std::cout << "The order " << orderId << " is already started." << std::endl;
        }
    }
    else
    {
        std::cout << "Order " << orderId << " not found." << std::endl;
    }

    file.close();
}
std::vector<std::string> OrderManagement::GetKeysFromInventoryFile(const std::string &fileName)
{
    std::ifstream inventoryFile(fileName);
    if (!inventoryFile.is_open())
    {
        std::cerr << "Problem with opening the input file!" << std::endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> elComponentIds;
    std::string line;
    while (std::getline(inventoryFile, line))
    {
        std::istringstream inventoryStringStream(line);
        std::string elCompId, minValue;

        if (inventoryStringStream >> elCompId >> minValue)
        {
            elComponentIds.push_back(elCompId);
        }
    }

    inventoryFile.close();
    return elComponentIds;
}
std::vector<std::string> OrderManagement::GetOrderIds(const std::string &fileName)
{
    std::ifstream ordersFile(fileName);
    if (!ordersFile.is_open())
    {
        std::cerr << "Problem with opening the input file!" << std::endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> orderIds;
    std::string line;
    bool foundClientKey = false;

    while (std::getline(ordersFile, line))
    {
        std::istringstream ordersStringStream(line);
        std::string key, value;
        if (ordersStringStream >> key)
        {
            std::getline(ordersStringStream, value);
            if (key == "client:")
            {
                foundClientKey = true;
            }
            else if (foundClientKey && key == "id:")
            {
                value = TrimInFile(value);
                orderIds.push_back(value);
                foundClientKey = false;
            }
        }
    }
    ordersFile.close();
    return orderIds;
}
std::vector<int> OrderManagement::GetValuesFromInventoryFile(const std::string &fileName)
{
    std::ifstream inventoryFile(fileName);
    if (!inventoryFile.is_open())
    {
        std::cerr << "Problem with opening the file!" << std::endl;
        return std::vector<int>();
    }

    std::vector<int> minValues;
    std::string line;
    while (std::getline(inventoryFile, line))
    {
        std::string componentId;
        int value;
        std::istringstream iss(line);
        if (iss >> componentId >> value)
        {
            minValues.push_back(value);
        }
    }

    inventoryFile.close();
    return minValues;
}
bool OrderManagement::CompareCountOfResourcesWithAvailableOnesInInventory()
{
    std::vector<std::vector<std::string>> orderComponents = SplitComponentsFromOrders();
    std::vector<std::map<std::string, int>> componentCountsPerOrder = GetComponentCountsPerOrder();
    std::vector<std::string> inventoryKeys = GetKeysFromInventoryFile(INVENTORY_FILENAME);
    std::vector<int> values = GetValuesFromInventoryFile(INVENTORY_FILENAME);

    static int orderCounter = 0;
    if (orderCounter >= componentCountsPerOrder.size())
    {
        orderCounter = 0;
    }

    const auto &componentCounts = componentCountsPerOrder[orderCounter];
    orderCounter++;

    for (const auto &componentCount : componentCounts)
    {
        const std::string &componentId = componentCount.first;
        int count = componentCount.second;

        int inventoryIndex = GetIndexFromComponentId(componentId, inventoryKeys);
        if (inventoryIndex != -1 && values[inventoryIndex] < count)
        {
            return false;
        }
    }
    return true;
}
void OrderManagement::ModifyStatusToPendingInOrder(unsigned int orderIndex)
{
    std::ifstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cerr << "Problem with opening the file!" << std::endl;
        return;
    }

    std::string line;
    std::string updatedOrderContent;
    unsigned int currentOrderIndex = 0;
    bool foundOrder = false;

    while (std::getline(file, line))
    {
        if (line.find("status: unprocessed") != std::string::npos)
        {
            if (currentOrderIndex == orderIndex)
            {
                foundOrder = true;
                line = "status: pending";
            }
            currentOrderIndex++;
        }
        updatedOrderContent += line + '\n';
    }

    file.close();

    if (foundOrder)
    {
        std::ofstream outputFile(ORDERS_FILENAME);
        if (outputFile.is_open())
        {
            outputFile << updatedOrderContent;
            outputFile.close();
        }
        else
        {
            std::cerr << "Problem with opening the file for writing!" << std::endl;
        }
    }
}
void OrderManagement::ModifyStatusToPrintingInOrder(unsigned int orderIndex)
{
    std::ifstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cerr << "Problem with opening the file!" << std::endl;
        return;
    }

    std::string line;
    std::string updatedOrderContent;
    unsigned int currentOrderIndex = 0;
    bool foundOrder = false;

    while (std::getline(file, line))
    {
        if (line.find("status: unprocessed") != std::string::npos)
        {
            if (currentOrderIndex == orderIndex)
            {
                foundOrder = true;
                line = "status: printing";
            }
            currentOrderIndex++;
        }
        updatedOrderContent += line + '\n';
    }

    file.close();

    if (foundOrder)
    {
        std::ofstream outputFile(ORDERS_FILENAME);
        if (outputFile.is_open())
        {
            outputFile << updatedOrderContent;
            outputFile.close();
        }
        else
        {
            std::cerr << "Problem with opening the file for writing!" << std::endl;
        }
    }
}
void OrderManagement::ModifyStatusFromPendingToPrintingInOrder(unsigned int orderIndex)
{
    std::ifstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cerr << "Problem with opening the file!" << std::endl;
        return;
    }

    std::string line;
    std::string updatedOrderContent;
    unsigned int currentOrderIndex = 0;
    bool foundOrder = false;

    while (std::getline(file, line))
    {
        if (line.find("status: pending") != std::string::npos)
        {
            if (currentOrderIndex == orderIndex)
            {
                foundOrder = true;
                line = "status: printing";
            }
            currentOrderIndex++;
        }
        updatedOrderContent += line + '\n';
    }

    file.close();

    if (foundOrder)
    {
        std::ofstream outputFile(ORDERS_FILENAME);
        if (outputFile.is_open())
        {
            outputFile << updatedOrderContent;
            outputFile.close();
        }
        else
        {
            std::cerr << "Problem with opening the file for writing!" << std::endl;
        }
    }
}
bool OrderManagement::CheckResourceAvailability()
{
    std::vector<std::string> orders = SplitOrders();
    unsigned int orderCount = orders.size();

    if (orderCount == 0)
    {
        std::cout << "No orders available." << std::endl;
        return false;
    }

    unsigned int orderIndex = 1;
    bool resourceAvailable = false;
    bool printResourceMessage = true;

    while (orderIndex <= orderCount && !resourceAvailable)
    {
        if (CompareCountOfResourcesWithAvailableOnesInInventory())
        {
            if (printResourceMessage)
            {
                std::cout << "You have enough resources to place the order." << std::endl;
                printResourceMessage = false;
            }
            ModifyStatusToPrintingInOrder(orderIndex);
            TransferOrderToPrintJobs();
            DeductFromInventory();
            resourceAvailable = true;
        }
        else
        {
            if (printResourceMessage)
            {
                std::cout << "You don't have enough resources to place the order." << std::endl;
                printResourceMessage = false;
            }
            ModifyStatusToPendingInOrder(orderIndex);
        }
        orderIndex++;
    }
    RemovePrintingOrders();
    return resourceAvailable;
}
void OrderManagement::TransferOrderToPrintJobs()
{
    std::ifstream orderFile(ORDERS_FILENAME);
    if (!orderFile.is_open())
    {
        std::cerr << "Problem with opening the order file!" << std::endl;
        return;
    }

    std::ofstream printJobsFile(PRINTJOBS_FILENAME, std::ios::app);
    if (!printJobsFile.is_open())
    {
        std::cerr << "Problem with opening the print jobs file!" << std::endl;
        orderFile.close();
        return;
    }
    std::vector<std::string> orders = SplitOrders();

    static int orderCounter = 1;
    if (orderCounter <= orders.size())
    {
        std::string order = orders[orderCounter - 1];

        std::istringstream ordersStringStream(order);
        std::string line;
        std::string orderId;

        std::getline(ordersStringStream, line);

        if (std::getline(ordersStringStream, line))
        {
            unsigned int colPosition = line.find(": ");
            if (colPosition != std::string::npos)
            {
                orderId = line.substr(colPosition + 2);
                orderId = TrimInFile(orderId);
            }
        }
        printJobsFile << "id: " << orderCounter << std::endl;
        printJobsFile << "name: " << orderId << std::endl;

        orderCounter++;
    }

    orderFile.close();
    printJobsFile.close();
}
int OrderManagement::GetIndexFromComponentId(const std::string &componentId, const std::vector<std::string> &inventoryKeys)
{
    for (int i = 0; i < inventoryKeys.size(); i++)
    {
        if (inventoryKeys[i] == componentId)
        {
            return i;
        }
    }
    return -1;
}
void OrderManagement::DeductFromInventory()
{
    static unsigned int currentOrderIndex = 0;
    std::ifstream file(INVENTORY_FILENAME);
    std::vector<std::string> lines;
    std::vector<int> minValues = GetValuesFromInventoryFile(INVENTORY_FILENAME);
    std::vector<std::map<std::string, int>> componentCountsPerOrder = GetComponentCountsPerOrder();
    std::vector<std::string> inventoryKeys = GetKeysFromInventoryFile(INVENTORY_FILENAME);

    if (!file.is_open())
    {
        std::cerr << "Problem with opening the inventory file!" << std::endl;
        return;
    }

    const auto &componentCounts = componentCountsPerOrder[currentOrderIndex];
    std::cout << currentOrderIndex << std::endl;
    for (const auto &componentCount : componentCounts)
    {
        const std::string &componentId = componentCount.first;
        int count = componentCount.second;

        int inventoryIndex = GetIndexFromComponentId(componentId, inventoryKeys);
        if (inventoryIndex != -1)
        {
            minValues[inventoryIndex] -= count;
        }
        else
        {
            std::cerr << "Component ID not found in inventory: " << componentId << std::endl;
        }
    }

    std::string line;
    for (int i = 0; i < minValues.size(); i++)
    {
        line = inventoryKeys[i] + " " + std::to_string(minValues[i]);
        lines.push_back(line);
    }
    file.close();

    std::ofstream inventoryFile(INVENTORY_FILENAME);
    if (!inventoryFile.is_open())
    {
        std::cerr << "Problem with opening the inventory file for writing!" << std::endl;
        return;
    }

    for (const auto &line : lines)
    {
        inventoryFile << line << std::endl;
    }
    inventoryFile.close();

    currentOrderIndex++;
}
void OrderManagement::TransferOrderToHistory()
{
    std::ifstream productionFile(PRODUCTION_FILENAME);
    if (!productionFile.is_open())
    {
        std::cerr << "Problem with opening the order file!" << std::endl;
        return;
    }

    std::ofstream history(HISTORY_FILENAME, std::ios::app);
    if (!history.is_open())
    {
        std::cerr << "Problem with opening the print jobs file!" << std::endl;
        productionFile.close();
        return;
    }

    std::string line;
    while (std::getline(productionFile, line))
    {
        history << line << std::endl;
    }
    productionFile.close();
    history.close();

    ТеxtFileUtils::ClearFile(PRODUCTION_FILENAME);
}
void OrderManagement::AddElectricComponent(const std::string &fileName)
{
    std::ifstream electricCompFile(fileName);
    if (!electricCompFile.is_open())
    {
        std::cerr << "Problem with opening the order file!" << std::endl;
        return;
    }
    std::vector<std::string> extractedData = ExtractElComponentsFromFile();
}
std::vector<std::string> OrderManagement::GetIdsFromElectricComponentsLibrary(const std::string &filename)
{
    std::ifstream elComomentFile(filename);
    if (!elComomentFile.is_open())
    {
        std::cerr << "Problem with opening the input file!" << std::endl;
        return std::vector<std::string>();
    }

    std::vector<std::string> ids;
    std::string line;
    while (std::getline(elComomentFile, line))
    {
        std::string trimmedLine = TrimInFile(line);
        if (!trimmedLine.empty())
        {
            std::istringstream elCompStringStream(trimmedLine);
            std::string id;
            std::getline(elCompStringStream, id, ';');
            ids.push_back(id);
        }
    }

    elComomentFile.close();
    return ids;
}
std::vector<std::string> OrderManagement::ExtractValuesFromLine(const std::string &line)
{
    std::vector<std::string> values;
    std::istringstream linesStringStream(line);
    std::string key, value;

    if (linesStringStream >> key >> value)
    {
        value = TrimInFile(value);
        values.push_back(value);
    }

    return values;
}
std::vector<std::string> OrderManagement::ExtractElComponentsFromFile()
{
    static unsigned int currentOrderIndex = 0;
    std::vector<std::string> data;
    std::string line;

    std::ofstream elFile(ELECTRIC_COMPONENT_FILENAME, std::ios::app);

    if (currentOrderIndex < m_hardwareComponents.size())
    {
        std::stringstream hCompStream(m_hardwareComponents[currentOrderIndex]);

        while (std::getline(hCompStream, line))
        {
            std::string trimmedLine = TrimInFile(line);
            if (trimmedLine.substr(0, 3) == "id:")
            {
                data = ExtractValuesFromLine(trimmedLine);
            }
            else if (trimmedLine.substr(0, 6) == "width:" || trimmedLine.substr(0, 7) == "height:" || trimmedLine.substr(0, 5) == "pins:")
            {
                std::vector<std::string> values = ExtractValuesFromLine(trimmedLine);
                for (int i = 0; i < values.size(); i++)
                {
                    std::string formattedValue = values[i];
                    formattedValue.erase(std::remove(formattedValue.begin(), formattedValue.end(), '('), formattedValue.end());
                    formattedValue.erase(std::remove(formattedValue.begin(), formattedValue.end(), ')'), formattedValue.end());

                    bool replaceComma = false;
                    for (int j = 0; j < formattedValue.size(); j++)
                    {
                        if (formattedValue[j] == ',')
                        {
                            if (replaceComma)
                            {
                                formattedValue[j] = '|';
                            }
                            replaceComma = !replaceComma;
                        }
                    }
                    data.push_back(formattedValue);
                }
            }
        }
        for (auto &d : data)
        {
            elFile << d << ';';
        }
        elFile << std::endl;
        elFile.close();
    }
    currentOrderIndex++;
    if (currentOrderIndex >= m_hardwareComponents.size())
    {
        currentOrderIndex = 0;
    }

    return data;
}
void OrderManagement::AddHardwareComponent(const std::string &fileName)
{
    std::ifstream hardwareCompFile(fileName);
    if (!hardwareCompFile.is_open())
    {
        std::cerr << "Problem with opening the order file!" << std::endl;
        return;
    }
    std::vector<std::string> extractedData = ExtractHardwareComponentFromFile();
}
std::string RemoveBracketsAndSpaces(const std::string &input)
{
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), '('), result.end());
    result.erase(std::remove(result.begin(), result.end(), ')'), result.end());
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    return result;
}
std::vector<std::string> OrderManagement::ExtractHardwareComponentFromFile()
{
    static unsigned int currentOrderIndex = 0;
    std::vector<std::string> data;
    std::vector<std::string> components = GetComponentsFromOrder();
    std::string line;
    unsigned int lineCount = 0;

    std::ofstream hardwareFile(HARDWARE_COMPONENT_FILENAME, std::ios::app);
    std::stringstream hCompStream(m_hardwareComponents[currentOrderIndex]);
    while (std::getline(hCompStream, line))
    {
        std::string trimmedLine = TrimInFile(line);
        trimmedLine.erase(std::remove(trimmedLine.begin(), trimmedLine.end(), '('), trimmedLine.end());
        trimmedLine.erase(std::remove(trimmedLine.begin(), trimmedLine.end(), ')'), trimmedLine.end());

        if (lineCount == 1 && trimmedLine.substr(0, 3) == "id:")
        {
            data = ExtractValuesFromLine(trimmedLine);
        }
        else if ((lineCount == 2 && trimmedLine.substr(0, 6) == "width:"))
        {
            std::vector<std::string> values = ExtractValuesFromLine(trimmedLine);
            for (int i = 0; i < values.size(); i++)
            {
                std::string formattedValue = values[i];
                formattedValue = RemoveBracketsAndSpaces(formattedValue);
                data.push_back(formattedValue);
            }
        }
        else if ((lineCount == 3 && trimmedLine.substr(0, 7) == "height:"))
        {
            std::vector<std::string> values = ExtractValuesFromLine(trimmedLine);
            for (int i = 0; i < values.size(); i++)
            {
                std::string formattedValue = values[i];
                formattedValue = RemoveBracketsAndSpaces(formattedValue);
                data.push_back(formattedValue);
            }
            auto firstElement = components.begin();
            for (auto it = components.begin(); it != components.end(); it++)
            {
                std::string formattedComponent = RemoveBracketsAndSpaces(*it);
                std::string &lastDataItem = data.back();
                if (it == firstElement)
                {
                    lastDataItem += ";" + formattedComponent;
                    continue;
                }
                if (!lastDataItem.empty())
                {
                    lastDataItem += "|" + formattedComponent;
                }
            }
        }
        else if (trimmedLine.substr(0, 12) == "connections:")
        {
            std::string trimmed = TrimInFile(trimmedLine.substr(12));
            trimmed = RemoveBracketsAndSpaces(trimmed);

            bool replaceComma = false;
            for (int j = 0; j < trimmed.size(); j++)
            {
                if (trimmed[j] == ',')
                {
                    if (replaceComma)
                    {
                        trimmed[j] = '|';
                    }
                    replaceComma = !replaceComma;
                }
            }
            data.push_back(trimmed);
        }

        lineCount++;
    }

    for (auto &item : data)
    {
        hardwareFile << item << ';';
    }
    hardwareFile << std::endl;
    hardwareFile.close();

    currentOrderIndex++;
    if (currentOrderIndex >= m_hardwareComponents.size())
    {
        currentOrderIndex = 0;
    }

    return data;
}
std::vector<std::string> OrderManagement::GetComponentsFromOrder()
{
    std::ifstream orderFile(ORDERS_FILENAME);
    if (!orderFile.is_open())
    {
        std::cerr << "Problem with opening the file: " << ORDERS_FILENAME << std::endl;
        return {};
    }

    std::vector<std::string> componentData;
    std::string line;
    bool isComponentSection = false;

    while (std::getline(orderFile, line))
    {
        std::string trimmedLine = TrimInFile(line);

        if (trimmedLine.substr(0, 12) == "connections:")
        {
            break;
        }
        if (isComponentSection)
        {
            componentData.push_back(trimmedLine);
        }
        if (trimmedLine.substr(0, 11) == "components:")
        {
            isComponentSection = true;
        }
    }

    orderFile.close();
    return componentData;
}
void OrderManagement::AddNewComponentIntoInventory()
{
    std::vector<std::string> libraryIds = GetIdsFromElectricComponentsLibrary(ELECTRIC_COMPONENT_FILENAME);
    std::vector<std::string> inventoryKeys = GetKeysFromInventoryFile(INVENTORY_FILENAME);
    std::vector<std::string> missingIds;

    for (const std::string &id : libraryIds)
    {
        if (std::find(inventoryKeys.begin(), inventoryKeys.end(), id) == inventoryKeys.end())
        {
            missingIds.push_back(id);
        }
    }

    std::ofstream inventoryFile(INVENTORY_FILENAME, std::ios::app);
    for (const std::string &id : missingIds)
    {
        inventoryFile << id << " 0" << std::endl;
    }
    inventoryFile.close();
}
std::vector<std::vector<std::string>> OrderManagement::SplitComponentsFromOrders()
{
    std::vector<std::vector<std::string>> elCompsInOrders;

    for (const auto &hardwareComp : m_hardwareComponents)
    {
        std::vector<std::string> currentElComponents;
        std::stringstream hCompStream(hardwareComp);
        std::string line;

        bool isComponentSection = false;
        std::string elComp;

        while (std::getline(hCompStream, line))
        {
            if (line.find("components:") != std::string::npos)
            {
                if (!elComp.empty())
                {
                    currentElComponents.push_back(elComp);
                    elComp = "";
                }
                isComponentSection = true;
                continue;
            }
            if (line.find("connections:") != std::string::npos)
            {
                isComponentSection = false;
                continue;
            }
            if (isComponentSection && !line.empty())
            {
                std::string key = line.substr(0, line.find(':'));
                key = TrimInFile(key);
                elComp += key + "\n";
            }
        }
        if (!elComp.empty())
        {
            currentElComponents.push_back(elComp);
        }

        elCompsInOrders.push_back(currentElComponents);
    }
    return elCompsInOrders;
}
std::vector<std::map<std::string, int>> OrderManagement::GetComponentCountsPerOrder()
{
    std::vector<std::map<std::string, int>> componentCountsPerOrder;
    std::vector<std::vector<std::string>> elCompsInOrders = SplitComponentsFromOrders();

    for (const auto &currentElComponents : elCompsInOrders)
    {
        std::map<std::string, int> componentCounts;
        for (const auto &components : currentElComponents)
        {
            std::stringstream compStream(components);
            std::string component;

            while (std::getline(compStream, component))
            {
                if (!component.empty())
                {
                    componentCounts[component]++;
                }
            }
        }
        componentCountsPerOrder.push_back(componentCounts);
    }
    return componentCountsPerOrder;
}
void OrderManagement::GroupOrdersByPriorityAndStatus()
{
    std::vector<std::string> orders = SplitOrders();
    bool hasPrintingOrder = false;

    auto compareOrders = [](const std::string &order1, const std::string &order2) {
        std::istringstream order1StringStream(order1);
        std::istringstream order2StringStream(order2);
        std::string line1, line2;
        std::string status1, status2, priority1, priority2;

        while (std::getline(order1StringStream, line1))
        {
            if (line1.find("priority:") != std::string::npos)
            {
                std::size_t priorityPosition = line1.find(":");
                if (priorityPosition != std::string::npos)
                {
                    priority1 = line1.substr(priorityPosition + 1);
                    priority1 = TrimInFile(priority1);
                    break;
                }
            }
        }
        while (std::getline(order1StringStream, line1))
        {
            if (line1.find("status:") != std::string::npos)
            {
                std::size_t statusPosition = line1.find(":");
                if (statusPosition != std::string::npos)
                {
                    status1 = line1.substr(statusPosition + 1);
                    status1 = TrimInFile(status1);
                    break;
                }
            }
        }

        while (std::getline(order2StringStream, line2))
        {
            if (line2.find("priority:") != std::string::npos)
            {
                std::size_t priorityPosition = line2.find(":");
                if (priorityPosition != std::string::npos)
                {
                    priority2 = line2.substr(priorityPosition + 1);
                    priority2 = TrimInFile(priority2);
                    break;
                }
            }
        }
        while (std::getline(order2StringStream, line2))
        {
            if (line2.find("status:") != std::string::npos)
            {
                std::size_t statusPosition = line2.find(":");
                if (statusPosition != std::string::npos)
                {
                    status2 = line2.substr(statusPosition + 1);
                    status2 = TrimInFile(status2);
                    break;
                }
            }
        }

        if (status1 == "pending" && status2 != "pending")
        {
            return true;
        }
        else if (status1 != "pending" && status2 == "pending")
        {
            return false;
        }
        else if (status1 == "unprocessed" && status2 != "unprocessed")
        {
            return false;
        }
        else if (status1 != "unprocessed" && status2 == "unprocessed")
        {
            return true;
        }
        else if (status1 == "pending" && status2 == "pending")
        {
            if (priority1 == "high" && priority2 == "normal")
                return true;
            else if (priority1 == "normal" && priority2 == "high")
                return false;
            else if (priority1 == "high" && priority2 == "high")
                return true;
            else if (priority1 == "normal" && priority2 == "normal")
                return true;
        }
        else if (status1 == "unprocessed" && status2 == "unprocessed")
        {
            if (priority1 == "high" && priority2 == "normal")
                return true;
            else if (priority1 == "normal" && priority2 == "high")
                return false;
            else if (priority1 == "high" && priority2 == "high")
                return true;
            else if (priority1 == "normal" && priority2 == "normal")
                return true;
        }

        return order1 >= order2;
    };
    std::sort(orders.begin(), orders.end(), compareOrders);

    if (!orders.empty())
    {
        std::ofstream sortedOrdersFile("../../txtFiles/sortedOrders.txt");

        for (const auto &order : orders)
        {
            std::istringstream iss(order);
            std::string line;
            std::string status;

            while (std::getline(iss, line))
            {
                if (line.find("status:") != std::string::npos)
                {
                    std::size_t statusPosition = line.find(":");
                    if (statusPosition != std::string::npos)
                    {
                        status = line.substr(statusPosition + 1);
                        status = TrimInFile(status);
                        break;
                    }
                }
            }
            if (status != "printing")
            {
                sortedOrdersFile << order << std::endl;
            }
            if (status == "printing")
            {
                hasPrintingOrder = true;
            }
        }
        sortedOrdersFile.close();
    }

    if (hasPrintingOrder)
    {
        RemovePrintingOrders();
    }
}
void OrderManagement::RemovePrintingOrders()
{
    std::ifstream ordersFile(ORDERS_FILENAME);
    if (!ordersFile.is_open())
    {
        std::cerr << "Failed to open file: " << ORDERS_FILENAME << std::endl;
        return;
    }

    std::vector<std::string> orders;
    std::string line;
    std::string currentOrder;
    bool printingOrder = false;

    while (std::getline(ordersFile, line))
    {
        if (line.empty())
        {
            if (!currentOrder.empty() && !printingOrder)
            {
                orders.push_back(currentOrder);
            }
            currentOrder.clear();
            printingOrder = false;
        }
        else
        {
            currentOrder += line + "\n";

            if (line.find("status:") != std::string::npos)
            {
                std::size_t statusPosition = line.find(":");
                if (statusPosition != std::string::npos)
                {
                    std::string status = line.substr(statusPosition + 1);
                    if (status.find("printing") != std::string::npos)
                    {
                        printingOrder = true;
                    }
                }
            }
        }
    }
    ordersFile.close();

    std::ofstream outputFile(ORDERS_FILENAME);
    if (!outputFile.is_open())
    {
        std::cerr << "Failed to open file for writing: " << ORDERS_FILENAME << std::endl;
        return;
    }
    for (const auto &order : orders)
    {
        outputFile << order << std::endl;
    }
    outputFile.close();
}
