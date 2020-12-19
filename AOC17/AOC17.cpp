#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include "../ScopeBasedTiming/ScopeBasedTimer.h"

struct ActiveCube
{
    int x = 0;
    int y = 0;
    int z = 0;
    int w = 0;

    bool IsNeigboursWith(ActiveCube x)
    {
        return IsNeigboursWith(x.x, x.y, x.z, x.w);
    }

    bool IsNeigboursWith(int xN, int yN, int zN, int wN = 0)
    {
        return (xN <= x + 1 && xN >= x - 1) && (yN <= y + 1 && yN >= y - 1) && (zN <= z + 1 && zN >= z - 1) && (wN <= w + 1 && wN >= w - 1);
    }

    bool operator==(ActiveCube const& other)
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
};

std::vector<ActiveCube> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;

    auto activeCubes = std::vector<ActiveCube>();
    int i = 0;
    while (std::getline(inputFile, line))
    {
        for (int j = 0; j < line.size(); j++)
        {
            if (line[j] == '#')
                activeCubes.push_back(ActiveCube{ i, j, 0, 0 });
        }
        i++;
    }

    return activeCubes;
}

std::vector<ActiveCube> CheckInactiveNeighbours(std::vector<ActiveCube> activeCubes)
{
    auto newCubes = std::vector<ActiveCube>();
    std::for_each(cbegin(activeCubes), cend(activeCubes), [&activeCubes, &newCubes](ActiveCube c) {
        for (int x = c.x - 1; x <= c.x + 1; x++)
        {
            for (int y = c.y - 1; y <= c.y + 1; y++)
            {
                for (int z = c.z - 1; z <= c.z + 1; z++)
                {
                    if (!(x == c.x && y == c.y && z == c.z) && std::find_if(cbegin(activeCubes), cend(activeCubes), [&x, &y, &z](ActiveCube c) {return x == c.x && y == c.y && z == c.z; }) == cend(activeCubes) &&
                        std::find_if(cbegin(newCubes), cend(newCubes), [&x, &y, &z](ActiveCube c) {return x == c.x && y == c.y && z == c.z; }) == cend(newCubes))
                    {
                        if (3 == std::count_if(cbegin(activeCubes), cend(activeCubes), [&x, &y, &z](ActiveCube cOther) {
                            return cOther.IsNeigboursWith(x, y, z);
                            }))
                            newCubes.push_back(ActiveCube{ x, y, z, 0 });
                    }
                }
            }
        }
        });

    return newCubes;
}

// an improvement would be to sort the cubes to get more efficient searching, but this also completes in a second so it's good enough for this excercise
std::vector<ActiveCube> CheckInactiveNeighboursFourD(std::vector<ActiveCube> activeCubes)
{
    auto newCubes = std::vector<ActiveCube>();
    std::for_each(cbegin(activeCubes), cend(activeCubes), [&activeCubes, &newCubes](ActiveCube c) {
        for (int x = c.x - 1; x <= c.x + 1; x++)
        {
            for (int y = c.y - 1; y <= c.y + 1; y++)
            {
                for (int z = c.z - 1; z <= c.z + 1; z++)
                {
                    for (int w = c.w - 1; w <= c.w + 1; w++)
                    {
                        if (!(x == c.x && y == c.y && z == c.z && w == c.w) && std::find_if(cbegin(activeCubes), cend(activeCubes), [&x, &y, &z, &w](ActiveCube c) {return x == c.x && y == c.y && z == c.z && w == c.w; }) == cend(activeCubes) &&
                            std::find_if(cbegin(newCubes), cend(newCubes), [&x, &y, &z, &w](ActiveCube c) {return x == c.x && y == c.y && z == c.z && w == c.w; }) == cend(newCubes))
                        {
                            if (3 == std::count_if(cbegin(activeCubes), cend(activeCubes), [&x, &y, &z, &w](ActiveCube cOther) {
                                return cOther.IsNeigboursWith(x, y, z, w);
                                }))
                                newCubes.push_back(ActiveCube{ x, y, z, w });
                        }
                    }
                }
            }
        }
        });

    return newCubes;
}

void PrintCubes(std::vector<ActiveCube> cubes, int t)
{
    std::cout << std::endl << "Turn = " << t << std::endl;
    auto [min, max] = std::accumulate(cbegin(cubes),
        cend(cubes),
        std::make_pair(std::make_tuple(cubes.front().x, cubes.front().x, cubes.front().x),
            std::make_tuple(cubes.front().x, cubes.front().x, cubes.front().x)),
        [](auto minMax, ActiveCube b) {
            if (b.x < std::get<0>(minMax.first))
                std::get<0>(minMax.first) = b.x;
            if (b.y < std::get<1>(minMax.first))
                std::get<1>(minMax.first) = b.y;
            if (b.z < std::get<2>(minMax.first))
                std::get<2>(minMax.first) = b.z;
            if (b.x > std::get<0>(minMax.second))
                std::get<0>(minMax.second) = b.x;
            if (b.y > std::get<1>(minMax.second))
                std::get<1>(minMax.second) = b.y;
            if (b.z > std::get<2>(minMax.second))
                std::get<2>(minMax.second) = b.z;
            return minMax;
        });

    auto [minX, minY, minZ] = min;
    auto [maxX, maxY, maxZ] = max;

    for (int z = minZ; z <= maxZ; z++)
    {
        std::cout << std::endl << "z = " << z << std::endl;
        for (int x = minX; x <= maxX; x++)
        {
            for (int y = minY; y <= maxY; y++)
            {
                if (std::find_if(cbegin(cubes), cend(cubes), [&x, &y, &z](ActiveCube c) {return x == c.x && y == c.y && z == c.z; }) != cend(cubes))
                    std::cout << 'X';
                else
                    std::cout << '.';
            }
            std::cout << std::endl;
        }
    }
}

int SimulatePocketSpace(std::vector<ActiveCube> activeCubes, int n, bool fourD = false)
{
    auto newCubes = std::vector<ActiveCube>();
    auto tmpActiveCubes = activeCubes;

    for (int t = 0; t < n; t++)
    {
        tmpActiveCubes = activeCubes;

        /*PrintCubes(activeCubes, t);*/

        newCubes = !fourD ? CheckInactiveNeighbours(activeCubes) : CheckInactiveNeighboursFourD(activeCubes);

        tmpActiveCubes.erase(std::remove_if(begin(tmpActiveCubes), end(tmpActiveCubes), [&newCubes, &activeCubes, numActiveNeighbours = 0](ActiveCube c) mutable {
            numActiveNeighbours = std::count_if(cbegin(activeCubes), cend(activeCubes), [&c](ActiveCube cOther) {
                if (c == cOther)
                    return false;
                return c.IsNeigboursWith(cOther);
                });
            if (!(numActiveNeighbours == 2 || numActiveNeighbours == 3))
                return true;
            return false;
            }), end(tmpActiveCubes));

        activeCubes = tmpActiveCubes;
        activeCubes.reserve(activeCubes.size() + newCubes.size());
        activeCubes.insert(end(activeCubes), cbegin(newCubes), cend(newCubes));
    }

    return activeCubes.size();
}

int main()
{
    auto activeCubes = GetInput("input.txt");

    int numActive, n = 6;
    {
        ScopeBasedTimer timer;

        numActive = SimulatePocketSpace(activeCubes, n);
    }

    std::cout << "Number active cubes after " << n << " iterations in three dimensions: " << numActive << std::endl;

    {
        ScopeBasedTimer timer;

        numActive = SimulatePocketSpace(activeCubes, n, true);
    }

    std::cout << "Number active cubes after " << n << " iterations in four dimenstions: " << numActive << std::endl;
}
