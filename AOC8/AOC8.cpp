#pragma once

#include "GameConsole.h"
#include <iostream>
#include <string>

int main()
{
    GameConsole gc;
    gc.LoadMemory("input.txt");
    gc.RunInstructions();

    std::cout << "The value of the accumulator before infinite loop is: " << gc.GetAcc() << std::endl;

    gc.RepairAndRunInstructions();

    std::cout << "The value of the accumulator after repairing infinite loop is: " << gc.GetAcc() << std::endl;
}