#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>

std::vector<std::vector<int>::const_iterator> FindSum(std::vector<int>::const_iterator begin, std::vector<int>::const_iterator end, int sum)
{
    bool found = false;
    auto result = end;
    int target;

    do 
    {
        target = sum - *begin;
        result = std::lower_bound(begin, end, target);

    } while (*result != target && ++begin != end);

    return std::vector<std::vector<int>::const_iterator>{ begin, result };
}

std::vector<std::vector<int>::const_iterator> FindSumN(std::vector<int>::const_iterator& begin, std::vector<int>::const_iterator& end, int sum, int n)
{
    if (n < 1)
    {
        return std::vector<std::vector<int>::const_iterator>{};
    }

    if (n == 1)
    {
        std::vector<int>::const_iterator result;
        result = std::lower_bound(begin, end, sum);
        
        if (*result != sum)
        {
            return std::vector<std::vector<int>::const_iterator>{ end };
        }
        else
        {
            return std::vector<std::vector<int>::const_iterator>{ result };
        }
    }

    if (n == 2)
    {
        return FindSum(begin, end, sum);
    }

    auto tBegin = begin;
    std::vector<std::vector<int>::const_iterator> result;
    int target;

    do
    {
        target = sum - *tBegin;
        result = FindSumN(begin, end, target, n - 1);
    } while (!std::all_of(cbegin(result), cend(result), [&end](auto i) { return i != end; }) && ++tBegin != end);

    result.push_back(tBegin);
    return result;
}

int main()
{
    std::ifstream inputFile;
    
    inputFile.open("input.txt");

    std::string line;
    std::vector<int> expenses;
    int expense;
    int target = 2020;

    while (std::getline(inputFile, line))
    {
        expense = stoi(line);
        if (expense < target)
        {
            expenses.push_back(expense);
        }
    }

    std::sort(begin(expenses), end(expenses));

    auto first = cbegin(expenses);
    auto second = cend(expenses);

    auto result = FindSum(first, second, target);

    std::cout << "First and second result in:" << std::endl;

    std::cout << *result[0] << " + " << *result[1] << " = " << *result[0] + (*result[1]) << std::endl;

    std::cout << "Multiplier: " << std::endl;

    std::cout << *result[0] << " * " << *result[1] << " = " << *result[0] * (*result[1]) << std::endl;

    result = FindSumN(first, second, target, 3);

    std::cout << "Sum of three:" << std::endl;

    std::cout << *result[0] << " + " << *result[1] << " + " << *result[2] << " = " << *result[0] + (*result[1]) + (*result[2]) << std::endl;

    std::cout << "Multiplier: " << *result[0] * (*result[1]) * (*result[2]) <<std::endl;

    result = FindSumN(first, second, target, 4);
}
