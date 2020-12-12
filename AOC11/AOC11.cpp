#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

enum class Position { EmptySeat, OccupiedSeat, Nothing };

std::vector<std::vector<Position>> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    std::vector<std::vector<Position>> waitingArea;

    while (std::getline(inputFile, line))
    {
        waitingArea.push_back(std::vector<Position>());
        std::for_each(begin(line), end(line), [&waitingArea](char c) {
            switch (c)
            {
            case 'L':
                waitingArea.back().push_back(Position::EmptySeat);
                break;
            case '.':
                waitingArea.back().push_back(Position::Nothing);
                break;
            case '#':
                waitingArea.back().push_back(Position::OccupiedSeat);
                break;
            }});
    }

    return waitingArea;
}

bool CheckHasTargetNum(std::vector<std::vector<Position>>& waitingArea, int iStart, int jStart, Position checkPos, int targetCheckPos, bool targetIsMax)
{
    int waISize = waitingArea.size();
    int waJSize = waitingArea.back().size();
    int numCheckPos = 0;

    for (int i = (iStart == 0) ? 0 : -1; i < ((iStart == waISize - 1) ? 1 : 2); i++)
    {
        for (int j = (jStart == 0) ? 0 : -1; j < ((jStart == waJSize - 1) ? 1 : 2); j++)
        {
            if (i == 0 && j == 0)
                continue;
            if (waitingArea[iStart + i][jStart + j] == checkPos)
            {
                numCheckPos++;
                if (numCheckPos > targetCheckPos && targetIsMax)
                    return false;
                
                if (numCheckPos == targetCheckPos && !targetIsMax)
                    return true;
                
            }
        }
    }

    return targetIsMax ? true : false;
}

int SeatsAfterStabilisedChaos(std::vector<std::vector<Position>>& waitingArea)
{
    bool changed = false;
    Position curPos = Position::Nothing;
    std::vector<std::vector<Position>> tempWaitingArea = waitingArea;

    do {
        changed = false;

        for (int i = 0; i < waitingArea.size(); i++)
        {
            for (int j = 0; j < waitingArea[0].size(); j++)
            {
                curPos = waitingArea[i][j];

                if (curPos == Position::EmptySeat && CheckHasTargetNum(waitingArea, i, j, Position::OccupiedSeat, 0, true))
                {
                    changed = true;
                    tempWaitingArea[i][j] = Position::OccupiedSeat;
                }
                else if (curPos == Position::OccupiedSeat && CheckHasTargetNum(waitingArea, i, j, Position::OccupiedSeat, 4, false))
                {
                    changed = true;
                    tempWaitingArea[i][j] = Position::EmptySeat;
                }
            }
        }

        waitingArea = tempWaitingArea;

    } while (changed);

    return std::accumulate(cbegin(waitingArea), cend(waitingArea), 0, [](int curSum, auto line) {
        return curSum + std::count_if(cbegin(line), cend(line), [](Position p) { return p == Position::OccupiedSeat; }); });
}

int main()
{
    auto waitingArea = GetInput("input.txt");

    int numSeatsAfterStabilesedChaos = SeatsAfterStabilisedChaos(waitingArea);

    std::cout << "Number of seats occupied after chaos has stabilised: " << numSeatsAfterStabilesedChaos << std::endl;
}
