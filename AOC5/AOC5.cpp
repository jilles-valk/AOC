#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

int ParseRowColumnToInt(std::string s)
{
    std::transform(cbegin(s), cend(s), begin(s), [](char c)
        {
            if (c == 'F' || c == 'L') 
                return '0';
            else 
                return '1';
        });

    return std::stoi(s, nullptr, 2);
}

struct BoardingPass
{
    int row = 0;
    int column = 0;

    BoardingPass(std::string rawPass)
    {
        row = ParseRowColumnToInt(rawPass.substr(0, 7));
        column = ParseRowColumnToInt(rawPass.substr(7, rawPass.size()));
    }

    int id() { return (row * 8) + column; };

    bool operator<(BoardingPass right)
    {
        return id() < right.id();
    }
};

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line;
    std::vector<BoardingPass> passes;

    while (std::getline(inputFile, line))
    {
        passes.push_back(BoardingPass(line));
    }

    auto maxId = *std::max_element(cbegin(passes), cend(passes), [](auto a, auto b) { return a < b; });

    std::cout << "The maximum id present is: " << maxId.id() << std::endl;

    std::sort(begin(passes), end(passes));

    int lastId = passes[0].id();

    auto onePastMySeat = *std::find_if(cbegin(passes) + 1, cend(passes), [differenceNotOne = true, &lastId] (BoardingPass p) mutable
        {
            differenceNotOne = p.id() - lastId != 1;
            lastId = p.id();
            return differenceNotOne;
        });

    int mySeatId = onePastMySeat.id() - 1;

    std::cout << "My seat id is: " << mySeatId << std::endl;
}

