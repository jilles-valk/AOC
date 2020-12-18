#include <iostream>
#include <fstream>
#include <sstream> 
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <limits>
#include "../ScopeBasedTiming/ScopeBasedTimer.h"

struct Instruction
{
    virtual void dummy() {};
};

struct Mem : Instruction
{
    int location;
    long long value;

    Mem(int l, long long v) : location(l), value(v) {};
};

struct Mask : Instruction
{
    long long oneMask;
    long long zeroMask;
    long long floatingMask;

    void SetMask(std::string s)
    {
        std::stringstream one;
        std::stringstream zero;
        std::stringstream floating;

        for (char c : s)
        {
            if (c == '1')
            {
                one << '1';
                zero << '0';
                floating << '0';
            }
            else if (c == '0')
            {
                one << '0';
                zero << '1';
                floating << '0';
            }
            else
            {
                one << '0';
                zero << '0';
                floating << '1';
            }
        }
        
        oneMask = std::stoll(one.str(), 0, 2);
        zeroMask = std::stoll(zero.str(), 0, 2);
        floatingMask = std::stoll(floating.str(), 0, 2);
    }
};

struct MemoryCell
{
    int location;
    long long value;
};

std::vector<Instruction*> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;

    std::vector<Instruction*> instructions;
    Mask* mask;
    Mem* mem;

    while (std::getline(inputFile, line))
    {
        if (line.substr(0, 4) == "mask")
        {
            mask = new Mask;
            mask->SetMask(line.substr(line.find('=') + 2));
            instructions.push_back(mask);
        }
        else if (line.substr(0, 3) == "mem")
        {
            mem = new Mem{ std::stoi(line.substr(line.find('[') + 1, line.find(']'))), std::stoll(line.substr(line.find('=') + 2)) };
            instructions.push_back(mem);
        }
    }

    return instructions;
}

long long RunProgram(std::vector<Instruction*> instructions)
{
    Mask curMask;
    Mask* perhapsMask;
    Mem* perhapsMem;
    std::map<int, long long> memory;

    for (Instruction* i : instructions)
    {
        perhapsMask = dynamic_cast<Mask*>(i);

        if (perhapsMask != 0)
        {
            curMask = *perhapsMask;
        }
        else
        {
            perhapsMem = dynamic_cast<Mem*>(i);
            if (perhapsMem != 0)
            {
                memory[perhapsMem->location] = ((perhapsMem->value & curMask.oneMask) | curMask.oneMask) | ~((~perhapsMem->value | curMask.zeroMask) | curMask.zeroMask);
            }
        }
    }
    return std::accumulate(cbegin(memory), cend(memory), 0LL, [](long long curVal, auto memVal) { return curVal + memVal.second; });
}

std::vector<long long> GetAdresses(Mask m, long long initialAdress)
{
    std::vector<long long> adresses = std::vector<long long>();
    long long adressOneZero = (initialAdress | m.oneMask) | (initialAdress & m.zeroMask);

    if (m.floatingMask == 0)
    {
        adresses.push_back(adressOneZero);
        return adresses;
    }
    
    adresses.push_back(adressOneZero & 1LL);

    size_t numAdresses;

    for (int lShift = 0; lShift <= 36; lShift++)
    {
        numAdresses = adresses.size();

        for (long long i = 0; i < numAdresses; i++)
        {
            if ((m.floatingMask & 1LL << lShift) != 0)
            {
                adresses[i] = adresses[i] | (1LL << lShift);
                adresses.push_back(adresses[i] & ~(1LL << lShift));
            }
            else
            {
                adresses[i] = adresses[i] | (adressOneZero & (1LL << lShift));
            }
        }
    }

    return adresses;
}

long long RunProgramVersionTwo(std::vector<Instruction*> instructions)
{
    Mask curMask;
    Mask* perhapsMask;
    Mem* perhapsMem;
    std::map<long long, long long> memory;
    std::vector<long long> adresses;

    for (Instruction* i : instructions)
    {
        perhapsMask = dynamic_cast<Mask*>(i);

        if (perhapsMask != 0)
        {
            curMask = *perhapsMask;
        }
        else
        {
            perhapsMem = dynamic_cast<Mem*>(i);
            if (perhapsMem != 0)
            {
                adresses = GetAdresses(curMask, perhapsMem->location);

                std::for_each(cbegin(adresses), cend(adresses), [&memory, &perhapsMem](long long adress) {
                    memory[adress] = perhapsMem->value;
                    });
            }
        }
    }
    return std::accumulate(cbegin(memory), cend(memory), 0LL, [](long long curVal, auto memVal) { return curVal + memVal.second; });
}

int main()
{
    auto instructions = GetInput("input.txt");
    {
        ScopeBasedTimer timer;
        long long sumMemory = RunProgram(instructions);

        std::cout << "The sum of all memory values is: " << sumMemory << std::endl;
    }

    {
        ScopeBasedTimer timer;
        long long sumMatchMemory = RunProgramVersionTwo(instructions);

        std::cout << "The sum of all memory values for version two is: " << sumMatchMemory << std::endl;
    }
}

