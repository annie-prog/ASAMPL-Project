#ifndef MY_CLASS_H
#define MY_CLASS_H

#include "TextFileUtils.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

class OrderManagement
{
  private:
    std::vector<std::string> m_hardwareComponents;
    enum class Status
    {
        Unprocessed,
        Pending,
        Printing
    } m_status;

  public:
    OrderManagement();

    void CheckIfTheOrderIsStartedOrNot(const std::string &orderId);
    bool CompareCountOfResourcesWithAvailableOnesInInventory();
    bool CheckResourceAvailability();

    std::vector<std::string> GetKeysFromInventoryFile(const std::string &fileName);
    std::vector<int> GetValuesFromInventoryFile(const std::string &fileName);
    void DeductFromInventory();
    void AddNewComponentIntoInventory();

    void TransferOrderToPrintJobs();
    void TransferOrderToHistory();

    std::vector<std::string> SplitOrders();
    std::vector<std::vector<std::string>> SplitComponentsFromOrders();
    std::vector<std::string> GetComponentsFromOrder();
    std::vector<std::map<std::string, int>> GetComponentCountsPerOrder();
    std::vector<std::string> GetOrderIds(const std::string &fileName);
    void GroupOrdersByPriorityAndStatus();
    void RemovePrintingOrders();
    int GetIndexFromComponentId(const std::string &componentId, const std::vector<std::string> &inventoryKeys);

    void ModifyStatusFromPendingToPrintingInOrder(unsigned int orderIndex);
    void ModifyStatusToPendingInOrder(unsigned int orderIndex);
    void ModifyStatusToPrintingInOrder(unsigned int orderIndex);

    void AddElectricComponent(const std::string &fileName);
    std::vector<std::string> GetIdsFromElectricComponentsLibrary(const std::string &fileName);

    std::vector<std::string> ExtractValuesFromLine(const std::string &line);
    std::vector<std::string> ExtractElComponentsFromFile();

    void AddHardwareComponent(const std::string &fileName);
    void AddDataToHardwareCompFile(const std::string &fileName, const std::vector<std::string> &hardwareComponent);
    std::vector<std::string> ExtractHardwareComponentFromFile();
};
#endif