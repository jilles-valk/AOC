#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <map>

std::map<std::string, std::string> ParsePassport(std::string::const_iterator beginPassport, std::string::const_iterator endPassport)
{
    std::map<std::string, std::string> passport;
    std::string::const_iterator colonPos;
    std::string::const_iterator spacePos;
    while (beginPassport < endPassport)
    {
        colonPos = std::find(beginPassport, endPassport, ':');
        spacePos = std::find(colonPos, endPassport, ' ');

        passport[std::string(beginPassport, colonPos)] = std::string(colonPos + 1, spacePos);

        beginPassport = spacePos + (spacePos == endPassport ? 0 : 1);
    }

    return passport;
}

bool CheckDigit(std::string s, int numDigits = 0, int min = INT32_MIN, int max = INT32_MAX)
{
    if (s.size() != numDigits && numDigits != 0)
        return false;

    if (!std::all_of(cbegin(s), cend(s), [](char c) { return isdigit(c); }))
        return false;

    int number = stoi(s);

    return min <= number && number <= max;
}

bool CheckLength(std::string s)
{
    int len = s.size();

    if (len < 2)
        return false;

    if (!std::all_of(cbegin(s), cend(s) - 2, [](char c) { return isdigit(c); }))
        return false;

    std::string val = std::string(cbegin(s), cend(s) - 2);

    if (*(cend(s) - 2) == 'c' && *(cend(s) - 1) == 'm')
    {
        if (!CheckDigit(val, 3, 150, 193))
            return false;
    }
    else if (*(cend(s) - 2) == 'i' && *(cend(s) - 1) == 'n')
    {
        if (!CheckDigit(val, 2, 59, 76))
            return false;
    }
    else
        return false;

    return true;
}

bool CheckHexColour(std::string s)
{
    if (s.size() != 7 || s[0] != '#')
        return false;

    return std::all_of(cbegin(s) + 1, cend(s), [](char c) { return isxdigit(c); });
}

bool CheckColour(std::string s)
{
    static std::vector<std::string> validColours{ "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };

    return std::find(cbegin(validColours), cend(validColours), s) != cend(validColours);
}

bool CheckPassportFields(std::map<std::string, std::string>& passport)
{
    static std::vector<std::string> required{ "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };
    static std::vector<std::string> optional{ "cid" };

    if (!std::all_of(cbegin(required), cend(required), [&passport](auto key) {return passport.count(key) > 0; }))
        return false;

    return true;
}

bool CheckPassportData(std::map<std::string, std::string> &passport)
{
    if (!CheckPassportFields(passport))
        return false;

    if (!CheckDigit(passport["byr"], 4, 1920, 2002))
        return false;
    if (!CheckDigit(passport["iyr"], 4, 2010, 2020))
        return false;
    if (!CheckDigit(passport["eyr"], 4, 2020, 2030))
        return false        ;
    if (!CheckLength(passport["hgt"]))
        return false;
    if (!CheckHexColour(passport["hcl"]))
        return false;
    if (!CheckColour(passport["ecl"]))
        return false;
    if (!CheckDigit(passport["pid"], 9))
        return false;

    return true;
}

int main()
{
    std::ifstream inputFile;

    inputFile.open("input.txt");

    std::string line, rawPassport;
    std::vector<std::map<std::string, std::string>> passports;

    while (std::getline(inputFile, line))
    {
        if (!line.empty())
        {
            if (!rawPassport.empty()) 
                rawPassport += " ";
            rawPassport += line;
        }
        
        if (line.empty() || inputFile.eof())
        {
            passports.push_back(ParsePassport(cbegin(rawPassport), cend(rawPassport)));

            rawPassport.clear();
        }
    }

    int numValid = std::count_if(cbegin(passports), cend(passports), [](auto p) { return CheckPassportFields(p); });

    std::cout << "Number of valid passports check fields = " << numValid << std::endl;

    numValid = std::count_if(cbegin(passports), cend(passports), [](auto p) { return CheckPassportData(p); });

    std::cout << "Number of valid passports check fields and data = " << numValid << std::endl;
}
