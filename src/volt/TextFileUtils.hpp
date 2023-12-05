#ifndef FILE_UTILS
#define FILE_UTILS
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static std::string ORDERS_FILENAME = "../../txtFiles/orders.txt";
static std::string SORTED_ORDERS_FILENAME = "../../txtFiles/sortedOrders.txt";
static std::string INVENTORY_FILENAME = "../../txtFiles/inventory.txt";
static std::string PRINTJOBS_FILENAME = "../../txtFiles/printJobs.txt";
static std::string PRODUCTION_FILENAME = "../../txtFiles/production.txt";
static std::string HISTORY_FILENAME = "../../txtFiles/history.txt";
static std::string ELECTRIC_COMPONENT_FILENAME = "../../library/electricCompsData.txt";
static std::string HARDWARE_COMPONENT_FILENAME = "../../library/hardWareCompsData.txt";

class ТеxtFileUtils
{
  public:
    static void ProcessOpenCommand(const std::vector<std::string> &tokens);
    static void ProcessCloseCommand();
    static void ProcessSaveCommand();
    static void ProcessSaveAsCommand(const std::vector<std::string> &tokens);
    static void ProcessExitCommand();
    static void ClearFile(const std::string &filename);
};
#endif