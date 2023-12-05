#include "TextFileUtils.hpp"

void ТеxtFileUtils::ProcessOpenCommand(const std::vector<std::string> &tokens)
{
    if (tokens.size() < 2)
    {
        std::cout << "Please provide a file path to open." << std::endl;
        return;
    }

    std::string filePath = tokens[1];
    std::ifstream file(filePath);
    if (!file)
    {
        std::cout << "File not found." << std::endl;
    }
    else
    {
        std::cout << "Successfully opened " << filePath << std::endl;
        std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        ORDERS_FILENAME = filePath;
    }
}
void ТеxtFileUtils::ProcessCloseCommand()
{
    std::fstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cout << "No file is currently open." << std::endl;
        return;
    }
    std::cout << "Successfully closed " << ORDERS_FILENAME << std::endl;
    ClearFile(ORDERS_FILENAME);
}
void ТеxtFileUtils::ProcessSaveCommand()
{
    std::fstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cout << "No file is currently open." << std::endl;
        return;
    }
    std::cout << "Successfully saved " << ORDERS_FILENAME << std::endl;
}
void ТеxtFileUtils::ProcessSaveAsCommand(const std::vector<std::string> &tokens)
{
    std::fstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cout << "No file is currently open." << std::endl;
        return;
    }
    if (tokens.size() < 2)
    {
        std::cout << "Please provide a file path to save as." << std::endl;
        return;
    }
    std::string newFilePath = tokens[1];

    std::ifstream current(ORDERS_FILENAME);
    std::string fileContent((std::istreambuf_iterator<char>(current)), std::istreambuf_iterator<char>());
    current.close();

    std::ofstream newFile(newFilePath);
    if (!newFile)
    {
        std::cout << "Error creating the new file." << std::endl;
        return;
    }
    newFile << fileContent;
    newFile.close();

    std::cout << "Successfully saved as " << newFilePath << std::endl;
}
void ТеxtFileUtils::ProcessExitCommand()
{
    std::fstream file(ORDERS_FILENAME);
    if (!file.is_open())
    {
        std::cout << "No file is currently open." << std::endl;
        ТеxtFileUtils::ClearFile(ORDERS_FILENAME);
        std::exit(0);
    }
    std::string response;
    std::cout << "Do you want to save changes before exiting? (yes/no): ";
    std::cin >> response;

    if (response == "yes")
    {
        ProcessSaveCommand();
        std::cout << "Exiting the program. Goodbye!" << std::endl;
        std::exit(0);
    }
    else if (response == "no")
    {
        std::cout << "Exiting the program without saving. Goodbye!" << std::endl;
        ТеxtFileUtils::ClearFile(ORDERS_FILENAME);
        std::exit(0);
    }
    else
    {
        std::cout << "Invalid response!" << std::endl;
        ТеxtFileUtils::ClearFile(ORDERS_FILENAME);
        std::exit(0);
    }
}
void ТеxtFileUtils::ClearFile(const std::string &filename)
{
    std::ofstream file(filename);
    file.close();
}