#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

std::vector<std::tuple<int, std::string>> GenerateColourVector(std::string description)
{
    std::vector<std::tuple<int, std::string>> bagColours;
    
    if (description == "no other bags.")
        return bagColours;

    size_t pos = 0, endNum = 0;
    int tNewBags = 0;

    while (pos < description.npos)
    {
        endNum = description.find(" ", pos);

        tNewBags = std::stoi(description.substr(pos, endNum - pos));

        pos = description.find("bag", endNum);

        bagColours.push_back(std::make_tuple(tNewBags, description.substr(endNum + 1, pos - endNum - 2)));

        pos = description.find(",", pos);

        if (pos != description.npos)
            pos += 2;
    }
    
    return bagColours;
}

std::map<std::string, std::vector<std::tuple<int, std::string>>> GetBags()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line, bagColour;
    bool updated = false;

    std::map<std::string, std::vector<std::tuple<int, std::string>>> bagRules;

    while (std::getline(inputFile, line))
    {
        bagColour = line.substr(0, line.find("bags") - 1);

        bagRules[bagColour] = GenerateColourVector(line.substr(line.find("contain") + 8, line.npos));

        if (bagColour == "shiny white")
            updated = updated;
    }

    return bagRules;
}

bool BagCanContainBag(std::map<std::string, std::vector<std::tuple<int, std::string>>>& bagRules, std::string curColour, std::string& colour)
{
    if (bagRules.count(curColour) < 1)
        return false;
    else
    {
        auto bagRule = bagRules.at(curColour);

        return std::any_of(cbegin(bagRule), cend(bagRule), [&bagRules, &colour](auto numColour) {
            if (std::get<1>(numColour) == colour)
                return true;
            if (BagCanContainBag(bagRules, std::get<1>(numColour), colour))
                return true;

            return false;
            });
    }
}

int NumBagsCanContainBag(std::map<std::string, std::vector<std::tuple<int, std::string>>> &bagRules, std::string bagColour)
{
    return std::accumulate(cbegin(bagRules), cend(bagRules), 0, [&bagRules, &bagColour](int count, auto bagRule) {
        if (BagCanContainBag(bagRules, std::get<0>(bagRule), bagColour))
            return count + 1;
        return count;
        });
}

int NumBagsRequiredInsideBag(std::map<std::string, std::vector<std::tuple<int, std::string>>> &bagRules, std::string bagColour)
{
    auto currentBag = bagRules.at(bagColour);

    if (currentBag.empty())
        return 0;

    return std::accumulate(cbegin(currentBag), cend(currentBag), 0, [&bagRules, &bagColour](int count, auto bagRule) {
        return count + std::get<0>(bagRule) * (NumBagsRequiredInsideBag(bagRules, std::get<1>(bagRule)) + 1);
        });
}

int main()
{
    auto bagRules = GetBags();

    std::cout << "Number of bags that can contain a shiny gold bag: " << NumBagsCanContainBag(bagRules, "shiny gold") << std::endl;

    std::cout << "Number of bags are required inside a shiny gold bag: " << NumBagsRequiredInsideBag(bagRules, "shiny gold") << std::endl;
}