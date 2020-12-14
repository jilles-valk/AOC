#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

enum class Position { EmptySeat, OccupiedSeat, Nothing };

struct Pos
{
    int x = 0;
    int y = 0;
    int direction = 90;
    Pos(int x, int y, int dir) : x(x), y(y), direction{ dir } {};

    void Turn(int deg)
    {
        direction = (direction + deg) % 360;
    }

    void Move(int amount)
    {
        x += direction == 90 ? amount : (direction == 270 ? -amount : 0);
        y += direction == 0 ? amount : (direction == 180 ? -amount : 0);
    }

    void Move(int amount, int dir)
    {
        x += dir == 90 ? amount : (dir == 270 ? -amount : 0);
        y += dir == 0 ? amount : (dir == 180 ? -amount : 0);
    }

    void Move(Pos p, int times)
    {
        x += times * (p.x - x);
        y += times * (p.y - y);
    }

    void Rotate(Pos p, int deg)
    {
        int tempX;
        for (int i = 90; i <= deg; i += 90)
        {
            tempX = x;
            x = (y - p.y) + p.x;
            y = -(tempX - p.x) + p.y;
        }
    }
};

std::vector<std::pair<char, int>> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    std::vector<std::pair<char, int>> directions;

    while (std::getline(inputFile, line))
    {
        directions.push_back(std::make_pair(line[0], stoi(line.substr(1))));
    }

    return directions;
}

int GetManhattanDistance(std::vector<std::pair<char, int>>::const_iterator beginDirections, std::vector<std::pair<char, int>>::const_iterator endDirections)
{
    Pos curPos = Pos(0, 0, 90);

    for (auto it = beginDirections; it < endDirections; it++)
    {
        switch ((*it).first)
        {
            case ('N'):
                curPos.Move((*it).second, 0);
                break;
            case ('E'):
                curPos.Move((*it).second, 90);
                break;
            case ('S'):
                curPos.Move((*it).second, 180);
                break;
            case ('W'):
                curPos.Move((*it).second, 270);
                break;
            case ('L'):
                curPos.Turn(360 - (*it).second);
                break;
            case ('R'):
                curPos.Turn((*it).second);
                break;
            case ('F'):
                curPos.Move((*it).second);
        }
    }

    return std::abs(curPos.x) + std::abs(curPos.y);
}

int GetManhattanDistanceWaypoint(std::vector<std::pair<char, int>>::const_iterator beginDirections, std::vector<std::pair<char, int>>::const_iterator endDirections, Pos waypoint)
{
    Pos curPos = Pos(0, 0, 90);
    Pos tempPos = curPos;

    for (auto it = beginDirections; it < endDirections; it++)
    {
        switch ((*it).first)
        {
        case ('N'):
            waypoint.Move((*it).second, 0);
            break;
        case ('E'):
            waypoint.Move((*it).second, 90);
            break;
        case ('S'):
            waypoint.Move((*it).second, 180);
            break;
        case ('W'):
            waypoint.Move((*it).second, 270);
            break;
        case ('L'):
            waypoint.Rotate(curPos, 360 - (*it).second);
            break;
        case ('R'):
            waypoint.Rotate(curPos, (*it).second);
            break;
        case ('F'):
            tempPos = curPos;
            curPos.Move(waypoint, (*it).second);
            waypoint.Move(tempPos, -(*it).second);
        }
    }

    return std::abs(curPos.x) + std::abs(curPos.y);
}


int main()
{
    auto directions = GetInput("input.txt");

    int partOne = GetManhattanDistance(cbegin(directions), cend(directions));

    std::cout << "Manhattan distance to end of directions: " << partOne << std::endl;

    int partTwo = GetManhattanDistanceWaypoint(cbegin(directions), cend(directions), Pos(10, 1, 90));

    std::cout << "Manhattan distance to end of directions with waypoint: " << partTwo << std::endl;

}
