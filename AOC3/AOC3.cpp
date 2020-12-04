#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

std::string FindStraightPath(std::vector<std::string>& map, int horStep, int verStep)
{
    int mapW = map[0].size();
    int mapH = map.size();
    std::string path = "";
    for (int hor = 0, ver = 0; ver < mapH; hor += horStep, ver += verStep)
    {
        path.push_back(map[ver][hor % mapW]);
    }
    return path;
}



int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line;
    std::vector<std::string> map;

    while (std::getline(inputFile, line))
    {
        map.push_back(line);
    }
    
    std::string path = FindStraightPath(map, 3, 1);

    int64_t numTrees = std::count(cbegin(path), cend(path), '#');

    std::cout << "Num trees encountered for 3 left 1 down = " << numTrees << std::endl;

    std::vector<std::vector<int>> steps{ {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2} };

    numTrees = std::accumulate(cbegin(steps), cend(steps), (int64_t) 1, [&map](int curVal, std::vector<int> curStep)
        { 
            std::string path = FindStraightPath(map, curStep[0], curStep[1]);
            int64_t numTrees = std::count(cbegin(path), cend(path), '#');

            return curVal * numTrees; });

    std::cout << "Multiplied num trees for several paths = " << numTrees << std::endl;
}
