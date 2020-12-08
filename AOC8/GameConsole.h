#pragma once
#include <string>
#include <vector>
#include <map>

enum class Operator { acc, jmp, nop };
static std::map<std::string, Operator> mOps{ {"acc", Operator::acc}, {"jmp", Operator::jmp}, {"nop", Operator::nop} };

struct Instruction
{
    Operator op;
    int value = 0;

    Instruction(std::string description)
    {
        op = mOps.at(description.substr(0, 3));

        value = std::stoi(description.substr(3, description.npos));
    }
};

class GameConsole
{
public:
    void LoadMemory(std::string fileName);
    bool RunInstructions();
    void RepairAndRunInstructions();
    int GetAcc() { return acc; };

private:
    int acc = 0;
    std::vector<Instruction> memory;
    bool CheckValidChange(std::vector<Instruction>::const_iterator operLocation, Operator changeTo);

};
