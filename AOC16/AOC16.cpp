#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <numeric>
#include <string>
#include <tuple>
#include "../ScopeBasedTiming/ScopeBasedTimer.h"

std::pair<std::pair<int, int>, std::pair<int, int>> ParseRule(std::string ruleString)
{
    size_t posA = ruleString.find('-');
    size_t posB = ruleString.find(' ') + 1;;

    int lowerA = stoi(ruleString.substr(0, posA));
    int upperA = stoi(ruleString.substr(posA + 1, posB - (posA + 1)));
    posA = ruleString.find('r') + 1;
    posB = ruleString.find('-', posA) + 1;
    int lowerB = stoi(ruleString.substr(posA, posB - posA));
    int upperB = stoi(ruleString.substr(posB));

    return std::make_pair(std::make_pair(lowerA, upperA), std::make_pair(lowerB, upperB));
}

std::tuple<std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>>, std::vector<int>, std::vector<std::vector<int>>> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    size_t colonPos;

    std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> rules;
    std::vector<int> myTicket;
    std::vector<std::vector<int>> otherTickets;

    while (std::getline(inputFile, line) && line != "your ticket:")
    {
        if (!line.empty())
        {
            colonPos = line.find(':');
            rules[line.substr(0, colonPos)] = ParseRule(line.substr(colonPos + 1));
        }
    }

    std::getline(inputFile, line);
    size_t seperatorPos, start = 0;

    while((seperatorPos = line.find(',', start)) != std::string::npos)
    {
        myTicket.push_back(stoi(line.substr(start, seperatorPos - start)));
        start = seperatorPos + 1;
    }

    myTicket.push_back(stoi(line.substr(start, seperatorPos - start)));

    while (std::getline(inputFile, line) && line != "nearby tickets:") {};

    while (std::getline(inputFile, line))
    {
        start = 0;
        otherTickets.push_back(std::vector<int>());
        while ((seperatorPos = line.find(',', start)) != std::string::npos)
        {
            otherTickets.back().push_back(stoi(line.substr(start, seperatorPos - start)));
            start = seperatorPos + 1;
        }
        otherTickets.back().push_back(stoi(line.substr(start, seperatorPos - start)));
    }

    return std::make_tuple(rules, myTicket, otherTickets);
}

bool CompliesWithRule(int num, std::pair<std::pair<int, int>, std::pair<int, int>> rule)
{
    if ((num >= rule.first.first && num <= rule.first.second) || (num >= rule.second.first && num <= rule.second.second))
        return true;

    return false;
}

int TicketScanningErrorRate(std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> rules, std::vector<std::vector<int>> &otherTIckets)
{
    int errorRate = 0;
    auto validTickets = std::vector<std::vector<int>>();
    bool validTicket = false;
    for (auto ticket : otherTIckets)
    {

        validTicket = std::all_of(cbegin(ticket), cend(ticket), [&rules, &errorRate, curRule = cbegin(rules), followingRules = false](int v) mutable {
            curRule = cbegin(rules);
            followingRules = false;
            while (!followingRules && curRule != cend(rules))
            {
                if (CompliesWithRule(v, (*curRule).second))
                    followingRules = true;

                curRule++;
            }

            if (!followingRules)
            {
                errorRate += v;
                return false;
            }
            return true;
            });

        if (validTicket)
            validTickets.push_back(ticket);
    }

    otherTIckets = validTickets;

    return errorRate;
}

std::vector<std::string> GetRuleOrder(std::map<std::string, std::pair<std::pair<int, int>, std::pair<int, int>>> & rules, std::vector<std::vector<int>> & otherTIckets)
{
    std::vector<std::pair<int, std::vector<std::string>>> ruleOrder;
    ruleOrder.reserve(rules.size());
    bool ruleValid;

    for (size_t i = 0; i < otherTIckets[0].size(); i++)
    {
        ruleOrder.push_back(std::make_pair(i, std::vector<std::string>()));

        std::for_each(cbegin(rules), cend(rules), [&otherTIckets, &i, &ruleValid, &ruleOrder](auto r) {
            ruleValid = true;
            for (size_t j = 0; j < otherTIckets.size(); j++)
            {
                if (!CompliesWithRule(otherTIckets[j][i], r.second))
                {
                    ruleValid = false;
                    break;
                }
            }

            if (ruleValid)
                ruleOrder[i].second.push_back(r.first);
            });
    }

    std::sort(begin(ruleOrder), end(ruleOrder), [](auto a, auto b) { return a.second.size() < b.second.size(); });

    for (auto roIt = begin(ruleOrder); roIt < end(ruleOrder); roIt++)
    {
        std::for_each(roIt + 1, end(ruleOrder), [&roIt](std::pair<int, std::vector<std::string>>& validRules) {
            validRules.second.erase(std::remove_if(begin(validRules.second), end(validRules.second), [&roIt](std::string const& rule) { return rule == (*roIt).second[0]; }), end(validRules.second));
            });
    }

    std::sort(begin(ruleOrder), end(ruleOrder), [](auto a, auto b) { return a.first < b.first; });

    std::vector<std::string> ruleOrderOut;
    ruleOrderOut.reserve(rules.size());

    std::for_each(cbegin(ruleOrder), cend(ruleOrder), [&ruleOrderOut](auto rules) { ruleOrderOut.push_back(rules.second.front());});

    return ruleOrderOut;
}

long long MultiplyTicketFields(std::vector<std::string> ruleOrder, std::vector<int> myTicket, std::string ruleStart = "")
{
    long long value = 1;
    for (int i = 0; i < ruleOrder.size(); i++)
    {
        if (ruleOrder[i].substr(0, ruleStart.size()) == ruleStart)
            value *= myTicket[i];
    }

    return value;
}

int main()
{
    auto[rules, myTicket, otherTickets] = GetInput("input.txt");
    int scanningErrorRate;
    {
        ScopeBasedTimer timer;
        scanningErrorRate = TicketScanningErrorRate(rules, otherTickets);
    }

    std::cout << "Ticket scanning error rate: " << scanningErrorRate << std::endl;

    long long partTwo;
    {
        ScopeBasedTimer timer;
        auto orderedRules = GetRuleOrder(rules, otherTickets);
        partTwo = MultiplyTicketFields(orderedRules, myTicket, "departure");
    }

    std::cout << "Departure fields multiplied: " << partTwo << std::endl;
}
