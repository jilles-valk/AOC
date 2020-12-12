#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

std::vector<int> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    std::vector<int> data;

    while (std::getline(inputFile, line))
    {
        data.push_back(std::stoi(line));
    }

    return data;
}

int NumNJoltDifferencesTimesNumMJoltDifferences(std::vector<int>::iterator beginData, std::vector<int>::iterator endData, int minStep, int maxStep)
{
    std::sort(beginData, endData);

    int numMinStep = 0;
    int numMaxStep = 0;

    for (auto itA = beginData, itB = beginData + 1; itB < endData; itA++, itB++)
    {
        if (*itB - *itA == minStep)
            numMinStep++;
        else if (*itB - *itA == maxStep)
            numMaxStep++;
        else if (*itB - *itA < minStep)
            return INT_MIN;
        else if (*itB - *itA > maxStep)
            return INT_MAX;
    }

    return numMinStep * numMaxStep;
}

long long NumDistinctArrangements(std::vector<int>::iterator beginData, std::vector<int>::iterator endData, int maxStep)
{
    long long numArrangements = 1;
    std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> branchesPrevCur{ std::make_pair(beginData, beginData) };
    std::vector<std::pair<std::vector<int>::iterator, std::vector<int>::iterator>> tempBranchesPrevCur; 
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> checkSamePrevIt;
    bool allCurSameNow = false, allCurSameLast = true;
    int maxNow = 0, maxLast = *beginData;

    while (true)
    {
        maxNow = 0;
        checkSamePrevIt = *begin(branchesPrevCur);
        allCurSameNow = true;

        std::for_each(begin(branchesPrevCur), end(branchesPrevCur), [&](auto prevCurIt) {
            auto it = prevCurIt.second;

            if (*it < maxLast || branchesPrevCur.size() == 1 || allCurSameLast)
            {
                while (++it < endData && *it <= *prevCurIt.second + maxStep)
                {
                    tempBranchesPrevCur.push_back(std::make_pair(prevCurIt.second, it));

                    if (*it != maxLast)
                        allCurSameNow = false;

                    maxNow = *it > maxNow ? *it : maxNow;
                }
            }
            else
            {
                tempBranchesPrevCur.push_back(prevCurIt);
            }});

        if (std::all_of(cbegin(tempBranchesPrevCur), cend(tempBranchesPrevCur), [end = *(endData - 1)](std::pair<std::vector<int>::iterator, std::vector<int>::iterator> tb){ return *tb.second == end; }))
        {
            numArrangements *= tempBranchesPrevCur.size();
            break;
        }
        
        if (tempBranchesPrevCur.size() > 1 && std::all_of(cbegin(tempBranchesPrevCur), cend(tempBranchesPrevCur), [first = *cbegin(tempBranchesPrevCur)](std::pair<std::vector<int>::iterator, std::vector<int>::iterator> tb) {
            return tb.first == first.first && tb.second == first.second; }))
        {
            numArrangements *= tempBranchesPrevCur.size();
            branchesPrevCur.clear();
            branchesPrevCur.push_back(tempBranchesPrevCur[0]);
            tempBranchesPrevCur.clear();
        }
        else 
        {
            branchesPrevCur = tempBranchesPrevCur;
            tempBranchesPrevCur.clear();
        }

        maxLast = maxNow;
        allCurSameLast = allCurSameNow;
    }

    return numArrangements;
}

int main()
{
    auto data = GetInput("input.txt");

    data.push_back(0);
    data.push_back(*std::max_element(cbegin(data), cend(data)) + 3);

    auto partOne = NumNJoltDifferencesTimesNumMJoltDifferences(begin(data), end(data), 1, 3);

    std::cout << "Num num jolt difference one times num jolt differences three in adapters is: " << partOne << std::endl;

    long long partTwo = NumDistinctArrangements(begin(data), end(data), 3);

    std::cout << "Number of different arrangements of adapters is: " << partTwo << std::endl;
}
