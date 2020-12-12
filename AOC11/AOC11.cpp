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

bool CheckDirection(std::vector<std::vector<Position>>& waitingArea, Position checkPos, Position notPos, int iStart, int jStart, int deltaI, int deltaJ)
{
    int i = 0, j = 0, maxI = 0, maxJ = 0, max = 0;

    if (deltaI < 0)
        maxI = iStart + 1;
    else if (deltaI > 0)
        maxI = waitingArea.size() - iStart;

    if (deltaJ < 0)
        maxJ = jStart + 1;
    else if (deltaJ > 0)
        maxJ = waitingArea[0].size() - jStart;

    max = deltaI == 0 ? maxJ : deltaJ == 0 ? maxI : std::min(maxI, maxJ);

    for (int t = 1; t < max; t++)
    {
        i = deltaI * t;
        j = deltaJ * t;

        if (waitingArea[iStart + i][jStart + j] == notPos)
            return false;

        if (waitingArea[iStart + i][jStart + j] == checkPos)
            return true;
    }

    return false;
}

bool CheckHasTargetNum(std::vector<std::vector<Position>>& waitingArea, int iStart, int jStart, Position checkPos, int targetCheckPos, bool targetIsMax, bool checkLine = false)
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

            if (checkLine ? CheckDirection(waitingArea, Position::OccupiedSeat, Position::EmptySeat, iStart, jStart, i , j ) : waitingArea[iStart + i][jStart + j] == checkPos)
            {
                numCheckPos++;
                if (numCheckPos > targetCheckPos && targetIsMax)
                    return false;
                
                if (numCheckPos == targetCheckPos && !targetIsMax)
                    return true;
                
            }
        }
    }

    return targetIsMax;
}

int SeatsAfterStabilisedChaos(std::vector<std::vector<Position>> waitingArea, int maxNumOccupied, bool checkLine = false)
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

                if (curPos == Position::EmptySeat && CheckHasTargetNum(waitingArea, i, j, Position::OccupiedSeat, 0, true, checkLine))
                {
                    changed = true;
                    tempWaitingArea[i][j] = Position::OccupiedSeat;
                }
                else if (curPos == Position::OccupiedSeat && CheckHasTargetNum(waitingArea, i, j, Position::OccupiedSeat, maxNumOccupied, false, checkLine))
                {
                    changed = true;
                    tempWaitingArea[i][j] = Position::EmptySeat;
                }

                //if (curPos == Position::OccupiedSeat)
                //    std::cout << 'X';
                //if (curPos == Position::EmptySeat)
                //    std::cout << 'L';
                //if (curPos == Position::Nothing)
                //    std::cout << '.';
                
                
            }
            //std::cout << std::endl;
        }

        waitingArea = tempWaitingArea;

        //std::cout << std::endl;

    } while (changed);

    return std::accumulate(cbegin(waitingArea), cend(waitingArea), 0, [](int curSum, auto line) {
        return curSum + std::count_if(cbegin(line), cend(line), [](Position p) { return p == Position::OccupiedSeat; }); });
}

int main()
{
    auto waitingArea = GetInput("input.txt");

    int numSeatsAfterStabilesedChaos = SeatsAfterStabilisedChaos(waitingArea, 4);

    std::cout << "Number of seats occupied after chaos has stabilised: " << numSeatsAfterStabilesedChaos << std::endl;

    numSeatsAfterStabilesedChaos = SeatsAfterStabilisedChaos(waitingArea, 5, true);

    std::cout << "Number of seats occupied after chaos has stabilised with new rules: " << numSeatsAfterStabilesedChaos << std::endl;
}
