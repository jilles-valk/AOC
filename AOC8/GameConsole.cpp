#include "GameConsole.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

void GameConsole::LoadMemory(std::string fileName)
{
    memory.reserve(1024);

    std::ifstream inputFile;

    inputFile.open(fileName);

    std::string line;

    while (std::getline(inputFile, line))
    {
        memory.emplace_back(line);
    }
}

bool GameConsole::RunInstructions()
{
    acc = 0;

    auto cursor = cbegin(memory);
    int cursorIndex = 0;

    std::set<int> passedInstructions;

    auto passedInstructionsIt = cend(passedInstructions);
    bool passedInstruction = false;

    while (cursor < cend(memory) && !passedInstructions.count(cursorIndex))
    {
        passedInstructions.insert(cursorIndex);

        switch ((*cursor).op) 
        {
        case Operator::acc:
            acc += (*cursor).value;
            cursor++;
            break;
        case Operator::jmp:
            cursor += (*cursor).value;
            break;
        case Operator::nop:
            cursor ++;
            break;
        default:
            cursor++;
        }

        cursorIndex = cursor - cbegin(memory);
    }

    return cursor >= cend(memory);
}

bool GameConsole::CheckValidChange(std::vector<Instruction>::const_iterator operLocation, Operator changeTo)
{
    if (changeTo == Operator::jmp && (*operLocation).value == 0)
        return false;

    if (changeTo == Operator::jmp && (operLocation + (*operLocation).value < cbegin(memory) || operLocation + (*operLocation).value > cend(memory)))
        return false;

    return true;
}

void GameConsole::RepairAndRunInstructions()
{
    auto curToChange = begin(memory);

    Operator oldOp, reverseOp;

    while ((curToChange = std::find_if(curToChange, end(memory), [](Instruction i) { return i.op == Operator::nop || i.op == Operator::jmp; })) < end(memory))
    {
        oldOp = (*curToChange).op;
        reverseOp = oldOp == Operator::nop ? Operator::jmp : Operator::nop;

        if (CheckValidChange(curToChange, reverseOp))
        {
            (*curToChange).op = reverseOp;
            if (RunInstructions())
                return;
            (*curToChange).op = oldOp;
        }

        curToChange++;
    }
}
