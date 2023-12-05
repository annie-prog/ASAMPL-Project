#include "AssetMonitor.hpp"
#include "menu.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Invalid command line arguments" << std::endl;
        return -1;
    }

    const std::string configPath = argv[1];
    const std::string inventoryPath = argv[2];

    run(configPath, inventoryPath);

    return 0;
}