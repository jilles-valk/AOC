#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line;
    std::vector<std::vector<std::string>> formAnswers;
    bool nextNewGroup = true;

    while (std::getline(inputFile, line))
    {
        if (!nextNewGroup)
        {
            nextNewGroup = line.empty();

            if (nextNewGroup)
                continue;
        }

        if (nextNewGroup)
        {
            formAnswers.push_back(std::vector<std::string> { line });
            nextNewGroup = false;
        }
        else
            formAnswers.back().push_back(line);
    }

    int numDistinctAnswers = std::accumulate(cbegin(formAnswers), cend(formAnswers), 0, [groupAnswersString = std::string()](int curVal, std::vector<std::string> groupAnswers) mutable {
        groupAnswersString = std::accumulate(cbegin(groupAnswers), cend(groupAnswers), std::string());

        std::sort(begin(groupAnswersString), end(groupAnswersString));

        return curVal + std::count_if(cbegin(groupAnswersString), cend(groupAnswersString), [lastVal = ' ', isDifferent = false] (auto c) mutable {
            isDifferent = c != lastVal;
            lastVal = c;
            return isDifferent;
        });
    });

    std::cout << "Sum of distinct answers in groups is: " << numDistinctAnswers << std::endl;

    int sumNumAllAnswered = std::accumulate(cbegin(formAnswers), cend(formAnswers), 0, [curAllAnswered = std::string()](int curSumNumAllAnswered, std::vector<std::string> groupAnswers) mutable {
        std::sort(begin(groupAnswers[0]), end(groupAnswers[0]));

        curAllAnswered = std::accumulate(cbegin(groupAnswers) + 1, cend(groupAnswers), groupAnswers[0], [] (std::string curAllAnswered, std::string personAnswers) {
            std::sort(begin(personAnswers), end(personAnswers));
            auto it = std::set_intersection(cbegin(curAllAnswered), cend(curAllAnswered), cbegin(personAnswers), cend(personAnswers), begin(curAllAnswered));

            curAllAnswered.resize(it - curAllAnswered.begin());

            return curAllAnswered;
        });
            
        return curSumNumAllAnswered + curAllAnswered.size();
    });

    std::cout << "Sum of all answered questions per groups is: " << sumNumAllAnswered << std::endl;
}
