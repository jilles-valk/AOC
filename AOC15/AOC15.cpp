#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "../ScopeBasedTiming/ScopeBasedTimer.h"

struct LastCurPos
{
    int last = -1;
    int cur = -1;
};

int NumberAtNthTurn(std::vector<int>& startingNumbers, int n)
{
    std::map<int, LastCurPos> numAndLastPos;

    for (int i = 0; i < startingNumbers.size(); i++)
        numAndLastPos[startingNumbers[i]] = LastCurPos{ -1, i + 1 };


    int lastNum = startingNumbers.back();
    int newNum;

    for (int i = startingNumbers.size() + 1; i <= n; i++)
    {
        if (numAndLastPos[lastNum].last == -1)
        {
            numAndLastPos[0] = LastCurPos{ numAndLastPos.count(0) > 0 ? numAndLastPos[0].cur : -1, i};
            lastNum = 0;
        }
        else
        {
            newNum = numAndLastPos[lastNum].cur - numAndLastPos[lastNum].last;
            numAndLastPos[newNum] = LastCurPos{ numAndLastPos.count(newNum) > 0 ? numAndLastPos[newNum].cur : -1, i };;
            lastNum = newNum;
        }
    }

    return lastNum;
}

int main()
{
    auto startingNumbers = std::vector<int>{ 6,13,1,15,2,0 };

    int n = 2020;

    int numAtN;

    {
        ScopeBasedTimer timer;
        numAtN = NumberAtNthTurn(startingNumbers, n);
    }

    std::cout << "Number at " << n << "th  turn: " << numAtN << std::endl;

    n = 30000000;

    {
        ScopeBasedTimer timer;
        numAtN = NumberAtNthTurn(startingNumbers, n);
    }

    std::cout << "Number at " << n << "th  turn: " << numAtN << std::endl;
}
