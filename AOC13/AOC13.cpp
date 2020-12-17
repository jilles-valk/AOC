#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>

struct BusLineTime
{
    long long departureTime = 0;
    long long lineNumber = 0;
    long long multiplier = 0;
    long long lineIndex = 0;
};

std::pair<int, std::vector<int>> GetInput(std::string inputFileName)
{
    std::ifstream inputFile(inputFileName);

    std::string line;
    std::pair<int, std::vector<int>> busNotes;

    std::getline(inputFile, line);

    busNotes.first = stoi(line);
    busNotes.second = std::vector<int>();

    std::getline(inputFile, line);

    size_t start = 0, pos = 0;

    std::string busLine;

    while ((pos = line.find(',', start)) != std::string::npos)
    {
        busLine = line.substr(start, pos - start);

        if (busLine == "x")
            busNotes.second.push_back(-1);
        else
            busNotes.second.push_back(stoi(busLine));

        start = pos + 1;
    }

    busNotes.second.push_back(stoi(line.substr(start)));

    return busNotes;
}

int EarliestBus(std::vector<int> busTimes, int arrivalTime)
{
    auto idWaitingTime = std::accumulate(cbegin(busTimes) + 1, cend(busTimes), std::make_pair(busTimes[0], busTimes[0] - (arrivalTime % busTimes[0])), [&arrivalTime](std::pair<int, int> curMin, int id) {
        if (id != -1)
            return curMin.second < id - (arrivalTime % id) ? curMin : std::make_pair(id, id - (arrivalTime % id));
        return curMin; });
    
    return idWaitingTime.first * idWaitingTime.second;
}

bool FindFirstConsecutiveDeparturesVTwo(BusLineTime& a, BusLineTime& b, long long startTime = 0)
{
    int delta = a.lineNumber * b.lineNumber;
    int t = startTime + (delta- startTime % delta) % delta;
    int i = (t / delta) + 1;
    int n = b.lineIndex - a.lineIndex;

    b.multiplier = i * a.lineNumber;
    a.multiplier = (b.lineNumber * b.multiplier) / a.lineNumber;
    a.departureTime = b.lineNumber * i * a.lineNumber;
    b.departureTime = a.departureTime + n;

    return true;
}


bool FindFirstConsecutiveDepartures(BusLineTime &a, BusLineTime &b, long long startTime = 0)
{
    int i = 1;
    long long t = 0;

    int n = b.lineIndex - a.lineIndex;

    if (a.lineNumber > b.lineNumber)
    {
        if (startTime > 0)
        {
            t = startTime + (a.lineNumber - startTime % a.lineNumber) % a.lineNumber;
        }
        else
            t = a.lineNumber;
        while (true)
        {
            if ((t + n) % b.lineNumber == 0)
                break;

            t += a.lineNumber;
        }
    }
    else
    {
        if (startTime > 0)
        {
            //t = startTime + (a.lineNumber - startTime % a.lineNumber) % a.lineNumber; // 
            t = startTime + (b.lineNumber - (startTime % b.lineNumber)) - n;
        }
        else
            t = b.lineNumber - n;
        while (true)
        {
            if (t % a.lineNumber == 0)
                break;

            t += b.lineNumber;
            i++;
        }
    }

    a.departureTime = t;
    a.multiplier = t / a.lineNumber;
    b.departureTime = t + n;
    b.multiplier = (t + n) / b.lineNumber;
    
    return true;
}

std::vector<BusLineTime> ParseBusTimes(std::vector<int> busTimes)
{
    std::vector<BusLineTime> busLineTimes = std::vector<BusLineTime>();
    auto it = cbegin(busTimes);

    while (it != cend(busTimes))
    {
        while (it != cend(busTimes) && *it == -1)
            it++;

        if (it == cend(busTimes))
            break;

        busLineTimes.push_back(BusLineTime{ 0, *it, 0, it - cbegin(busTimes) });
        it++;
    }

    return busLineTimes;
}

int TimeBussesLeaveOneMinuteDifference(std::vector<int> busTimes)
{
    std::vector<BusLineTime> busLineTimes = ParseBusTimes(busTimes);

    BusLineTime tmpA, tmpB;
    auto first = begin(busLineTimes);
    auto second = first + 1;
    long long startTime = 0;

    while (second != cend(busLineTimes))
    {
        FindFirstConsecutiveDeparturesVTwo(*first, *second, startTime);

        if (first > begin(busLineTimes) && (*first).departureTime - ((*first).lineIndex - (*(first - 1)).lineIndex) > (*(first - 1)).departureTime)
        {
            startTime = (*first).departureTime - ((*first).lineIndex - busLineTimes.front().lineIndex);
            first = begin(busLineTimes);
            second = first + 1;

            startTime = (*first).departureTime + 1;
           /* tmpA = *(first + 1);
            tmpB = *(second + 1);
            FindFirstConsecutiveDepartures(tmpA, tmpB, (*first).departureTime + ((*second).lineIndex - (*first).lineIndex) + 1);
            startTime = tmpA.departureTime - ((*second).lineIndex - (*first).lineIndex);*/
            /*if (startTime <= (*(first - 1)).departureTime)
                startTime = (*(first - 1)).departureTime + 1;*/
            //startTime = (*(first - 1)).departureTime + 1;
            //second = first;
            //first--;
        }
        else
        {
            startTime = (*second).departureTime;
            first = second;
            second++;
        }

        
    }

    return busLineTimes.front().departureTime;
}

int main()
{
    BusLineTime a = BusLineTime{ 0, 5, 0, 0 };
    BusLineTime b = BusLineTime{ 0, 6, 0, 1 };

    FindFirstConsecutiveDeparturesVTwo(a, b, 0);
    FindFirstConsecutiveDeparturesVTwo(a, b, 30);
    FindFirstConsecutiveDeparturesVTwo(a, b, 31);

    FindFirstConsecutiveDepartures(a, b, 0);
    FindFirstConsecutiveDepartures(a, b, 30);
    FindFirstConsecutiveDepartures(a, b, 31);

    auto data = GetInput("input.txt");

    int earliestBusIdTimesWaitingTime = EarliestBus(data.second, data.first);

    std::cout << "Earliest bus id times waiting time: " << earliestBusIdTimesWaitingTime << std::endl;

    int timeBussesLeaveConsecutive = TimeBussesLeaveOneMinuteDifference(data.second);

    std::cout << "Earliest time busses leave consecutive " << timeBussesLeaveConsecutive << std::endl;
}
