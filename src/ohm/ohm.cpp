#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
// to do list:
// 1) при добавяне на поръчка,ohm и volt не взаимодействат поради грешен синтаксис на кода
// 2) желателно е да се трият поръчки чрез id
// 3) При четене на подадени поръчки , част от първата поръчка се зарежда във втората.Това се
// дължи на "счупването" на вектора. 
struct Order
{
    std::string description;
    std::string priority;
    std::string status = "unprocessed";
};
void displayOrders(const std::vector<Order> &orders)
{
    std::cout << "Your orders:" << std::endl;
    for (const auto &order : orders)
    {
        std::cout << order.description << "(Priority: " << order.priority << ")" << std::endl;
    }
}
void saveOrdersToFile(const std::vector<Order> &orders)
{
    std::fstream OrderFile("orders.txt", std::fstream::in | std::fstream::out | std::fstream::app);
    if (OrderFile.is_open())
    {
        for (const auto &order : orders)
        {
            OrderFile << order.description << "priority: " << order.priority << '\n'
                      << "status: unprocessed";
            OrderFile << '\n';
        }
        OrderFile.close();
    }
    else
    {
        std::cerr << "Unable to save orders to file." << std::endl;
    }
}
void filterOrdersByPriority(const std::vector<Order> &orders, const std::string &priority)
{
    std::cout << "Orders filtered by priority '" << priority << "':" << std::endl;
    for (const auto &order : orders)
    {
        if (order.priority == priority)
        {
            std::cout << order.description << "(Priority: " << order.priority << ")" << std::endl;
        }
    }
}

int main()
{
    std::vector<Order> orders;
    std::ifstream OrderFile("orders.txt");
    if (OrderFile.is_open())
    {
        std::string line;
        while (std::getline(OrderFile, line))
        {
            size_t commaPos = line.find(",");
            if (commaPos != std::string::npos)
            {
                std::string description = line.substr(0, commaPos);
                std::string priority = line.substr(commaPos + 1);
                Order newOrder{description, priority};
                orders.push_back(newOrder);
            }
        }
        OrderFile.close();
    }

    std::cout << "Hello dear user! Feel free to inspect this program by following the instructions below!" << std::endl;
    std::string userInput;
    int choice = 0;
    while (true)
    {
        std::cout << "Please choose an option:" << std::endl;
        std::cout << "1. Add order" << std::endl;
        std::cout << "2. Delete order" << std::endl;
        std::cout << "3. Show orders" << std::endl;
        std::cout << "4. Filter orders by priority" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Input your choice: ";
        std::cin >> choice;
        std::cin.ignore();
        if (choice == 1)
        {
            std::string line;
            std::string order;
            std::string priority;
            while (std::getline(std::cin, line) && !line.empty())
            {
                order += line + '\n';
            }
            std::cout << "Enter the priority (normal or high): ";
            std::getline(std::cin, priority);
            Order newOrder{order, priority};
            orders.push_back(newOrder);
            std::cout << "Your order has been added." << std::endl;
        }
        else if (choice == 2)
        {
            std::cout << "Choose the number of the order you wish to delete: ";
            int orderNumber;
            std::cin >> orderNumber;
            std::cin.ignore();

            if (orderNumber >= 1 && orderNumber <= orders.size())
            {
                orders.erase(orders.begin() + orderNumber - 1);
                std::cout << "Order " << orderNumber << " has been deleted." << std::endl;
            }
            else
            {
                std::cout << "Invalid order number. Please try again." << std::endl;
            }
        }

        else if (choice == 3)
        {
            displayOrders(orders);
        }
        else if (choice == 4)
        {
            std::cout << "Enter priority (normal or high): ";
            std::getline(std::cin, userInput);
            filterOrdersByPriority(orders, userInput);
        }
        else if (choice == 5)
        {
            saveOrdersToFile(orders);
            break;
        }
        else
        {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}