#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

struct PWData
{
    int firstDigit;
    int secondDigit;
    char c;
    std::string password;

    PWData(std::string rawInput)
    {
        auto dashPos = std::find(cbegin(rawInput), cend(rawInput), '-');

        firstDigit = std::stoi(std::string(cbegin(rawInput), dashPos));

        auto spacePos = std::find(cbegin(rawInput), cend(rawInput), ' ');

        secondDigit = std::stoi(std::string(dashPos + 1, spacePos));

        c = *(spacePos + 1);

        password = std::string((spacePos + 4), cend(rawInput));
    }

    bool IsValidMinMax()
    {
        int countC = std::count(cbegin(password), cend(password), c);
        return firstDigit <= countC && countC <= secondDigit;
    }

    bool IsValidPos()
    {
        return (*(cbegin(password) + firstDigit - 1) == c) ^ (*(cbegin(password) + secondDigit - 1) == c);
    }
};

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line;
    std::vector<PWData> passwords;

    while (std::getline(inputFile, line))
    {
        passwords.push_back(PWData(line));
    }

    int numValid = std::count_if(cbegin(passwords), cend(passwords), [](auto p) { return p.IsValidMinMax(); });

    std::cout << "Number of valid passwords with min and max = " << numValid << std::endl;

    numValid = std::count_if(cbegin(passwords), cend(passwords), [](auto p) { return p.IsValidPos(); });

    std::cout << "Number of valid passwords with positions = " << numValid << std::endl;
}

