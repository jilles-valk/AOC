#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>

std::vector<long long> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    std::vector<long long> data;

    while (std::getline(inputFile, line))
    {
        data.push_back(std::stoll(line.c_str()));
    }

    return data;
}

std::vector<long long>::const_iterator FindNumNotSumPreviousNRec(std::map<long long, int> & countSumsPreviousN, std::vector<long long>::const_iterator beginData, std::vector<long long>::const_iterator curFirstData, std::vector<long long>::const_iterator endData, int n)
{
    if (curFirstData + n >= endData)
        return endData;

    // shift set to include new previous value
    if (curFirstData > beginData)
    {
        auto firstOneBeforeCurFirst = curFirstData - 1;
        auto b = curFirstData + n - 1;

        int sum;

        //remove sums for one before first of n before cur
        for (auto a = firstOneBeforeCurFirst + 1; a < b; a++)
        {
            sum = *a + *firstOneBeforeCurFirst;
            if (countSumsPreviousN.count(sum) > 0)
            {
                if (countSumsPreviousN.at(sum) > 1)
                    countSumsPreviousN.at(sum)--;
                else
                    countSumsPreviousN.erase(sum);
            }
        }

        b = curFirstData + n - 1;

        // insert sums for one before current
        for (auto a = curFirstData; a < b; a++)
        {
            sum = *a + *b;
            if (countSumsPreviousN.count(sum) < 1)
                countSumsPreviousN.insert(std::make_pair(*a + *b, 1));
            else
                countSumsPreviousN.at(sum)++;
        }
    }

    // check if current is sum of previous n
    if (countSumsPreviousN.count(*(curFirstData + n)) < 1)
        return curFirstData + n;
    else return FindNumNotSumPreviousNRec(countSumsPreviousN, beginData, ++curFirstData, endData, n);
}

std::vector<long long>::const_iterator FindNumNotSumPreviousN(std::vector<long long>::const_iterator beginData, std::vector<long long>::const_iterator endData, int n)
{
    if (beginData + n + 1 > endData)
        return endData;

    std::map<long long, int> countSumsPreviousN;

    auto cur = beginData + n;
    long long sum;
    for (auto a = beginData; a < cur; a++)
    {
        for (auto b = beginData; b < cur; b++)
        {
            if (b == a)
                continue;

            sum = *a + *b;
            if (countSumsPreviousN.count(sum) < 1)
                countSumsPreviousN.insert(std::make_pair(*a + *b, 1));
            else
                countSumsPreviousN.at(sum)++;
        }
    }

    return FindNumNotSumPreviousNRec(countSumsPreviousN, beginData, beginData, endData, n);
}

long long SumFirstLastContiguousSet(std::vector<long long>::const_iterator beginData, std::vector<long long>::const_iterator endData, long long target)
{
    auto beginConSec = beginData;
    auto curConSec = beginData;
    long long curSum = *beginConSec;

    while (curConSec < endData)
    {
        if (curSum == target)
            return *std::min_element(beginConSec, curConSec + 1) + *std::max_element(beginConSec, curConSec + 1);

        if (curSum < target)
            curSum += *(++curConSec);
        else
            curSum -= *(beginConSec++);
    }

    return INT64_MIN;
}

int main()
{
    auto data = GetInput("input.txt");
    int n = 25;
    auto itFirstNumberNotSum = FindNumNotSumPreviousN(cbegin(data), cend(data), n);

    if (itFirstNumberNotSum != cend(data))
        std::cout << "First number not sum of any previous " << n << ": " << *itFirstNumberNotSum << std::endl;
    else
        std::cout << "None found." << std::endl;

    long long sumFirstLastContiguousSet = SumFirstLastContiguousSet(cbegin(data), cend(data), *itFirstNumberNotSum);

    if (sumFirstLastContiguousSet != INT64_MIN)
        std::cout << "Sum min and max of contiguous set summing to above number: " << sumFirstLastContiguousSet << std::endl;
    else
        std::cout << "None found." << std::endl;
}
