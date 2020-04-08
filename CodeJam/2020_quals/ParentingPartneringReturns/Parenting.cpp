#include <iostream>
#include <vector>
#include <iostream>
#include <set>

typedef std::pair<int, int> Appt;


//#define DEBUG_MODE
#ifdef DEBUG_MODE
   #define DEBUG std::cout
#else
   #define DEBUG if(0) std::cout
#endif

Appt readAppt()
{
    int start;
    int stop;
    std::cin >> start >> stop;
    return std::make_pair(start, stop);
}

bool operator==(Appt const & lhs, Appt const & rhs)
{
    return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

bool operator!=(Appt const & lhs, Appt const & rhs)
{
    return ! (lhs == rhs);
}

bool operator>(Appt const & lhs, Appt const & rhs)
{
    if (lhs == rhs)
    {
        return false;
    }

    if (lhs.first > rhs.first)
    {
        return true;
    }
    if (lhs.first < rhs.first)
    {
        return false;
    }

    // First terms must be equal, compare the second now
    return (lhs.second > rhs.second);
}

bool operator<(Appt const & lhs, Appt const & rhs)
{
    if (lhs == rhs)
    {
        return false;
    }

    return !(lhs > rhs);
}

bool operator>=(Appt const & lhs, Appt const & rhs)
{
    return !(lhs < rhs);
}

bool operator<=(Appt const & lhs, Appt const & rhs)
{
    return !(lhs > rhs);
}

std::string apptToString(Appt const & a)
{
    std::string retVal;
    retVal += "[";
    retVal += std::to_string(a.first);
    retVal += ",";
    retVal += std::to_string(a.second);
    retVal += "]";

    return retVal;
}

void printApptList(std::set<Appt> const & apptList)
{
    for(auto curAppt : apptList)
    {
        DEBUG << apptToString(curAppt) << std::endl;
    }
}

bool doApptsOverlap(Appt const & lhs, Appt const & rhs)
{
    // Appt start time falls withing rhs
    if ( (lhs.first > rhs.first) && (lhs.first < rhs.second) )
    {
        DEBUG << "Appointments overlap.  Start time of " << apptToString(lhs) << " in the middle of " << apptToString(rhs) << std::endl;
        return true;
    }

    // Appt stop time falls within rhs
    if ( (lhs.second > rhs.first) && (lhs.second < rhs.second) )
    {
        DEBUG << "Appointments overlap.  End time of " << apptToString(lhs) << " in the middle of " << apptToString(rhs) << std::endl;
        return true;
    }

    // Appt completely covers rhs
    if ( (lhs.first <= rhs.first) && (lhs.second >= rhs.second) )
    {
        DEBUG << "Appointments overlap.  Appt " << apptToString(lhs) << " overlaps completely with " << apptToString(rhs) << std::endl;
        return true;
    }

    DEBUG << "Apoointments do not conflict: " << apptToString(lhs) << "  and " << apptToString(rhs) << std::endl;
    return false;
}

bool isApptInSchedule(Appt const & appt, std::set<Appt> schedule)
{
    return schedule.count(appt);
}

bool doesApptConflictWithSchedule(Appt possibleAppt, std::set<Appt> curAppts)
{
    for(auto singleCurAppt : curAppts)
    {
        if (doApptsOverlap(possibleAppt, singleCurAppt))
        {
            return true;
        }
    }

    return false;
}


bool isTimeWithinAnyAppt(int time, std::set<Appt> const & schedule)
{
    for(auto curAppt : schedule)
    {
        if ( (time > curAppt.first) && (time < curAppt.second) )
        {
            return true;
        }
    }

    return false;
}

std::vector<std::set<Appt> > divideSets(std::set<Appt> const & original)
{
    std::vector<std::set<Appt> > retVal;
    std::set<Appt> curWorkingSet;
    // Check the time after each appt, if see a time that does not conflict with any other appointments, we can divide and conquer

    for(auto curAppt : original)
    {
        curWorkingSet.emplace(curAppt);

        int endTime = curAppt.second;

        if (!isTimeWithinAnyAppt(endTime, original))
        {
            // We have a point in time where we can divide and conquer
            retVal.push_back(curWorkingSet);
            curWorkingSet.clear();
        }
    }

    DEBUG << "Divided set for CaC into " << retVal.size() << " smaller sets" << std::endl;
    return retVal;
}

bool isScheduleValid(std::set<Appt> const & schedule)
{
    // Do any appts in this set overlap with any other appts?

    for(auto curAppt : schedule)
    {
        for(auto testAgainst : schedule)
        {
            if (curAppt == testAgainst)
            {
                // Same appts, skip
                continue;
            }

            if (doApptsOverlap(curAppt, testAgainst))
            {
                return false;
            }
        }
    }

    return true;
}
void addJByRef(std::string & origString)
{
    // Adding 1 in binary
    // Start at right, if it is a 1(J), turn into C, repeat until we get to first
    // C, turn into J
    for(int i = origString.size() - 1; i >= 0; i--)
    {
        if (origString[i] == 'C')
        {
            origString[i] = 'J';
            return;
        }
        else
        {
            origString[i] = 'C';
        }
    }
}

std::string craftString(char letter, int numTimes)
{
    std::string retVal;
    for(int i = 0; i < numTimes; i++)
    {
        retVal += letter;
    }

    return retVal;
}

std::string getOrigOrderString(std::set<Appt> const & cSet,
                               std::set<Appt> const & jSet,
                               std::vector<Appt> const & origOrder)
{
    std::string retVal;

    for (auto curAppt : origOrder)
    {
        if (isApptInSchedule(curAppt, cSet))
        {
            retVal += "C";
        }
        else
        {
            retVal += "J";
        }
    }

    return retVal;
}

std::string findRandomSolution(std::set<Appt> const & orig, std::vector<Appt> const & origOrder)
{
    std::string solution = craftString('C', orig.size());
    std::string endCase = craftString('J', orig.size());

    while(solution != endCase)
    {
        DEBUG << "Trying random: " << solution << std::endl;

        std::set<Appt> cSet;
        std::set<Appt> jSet;
        int i = 0;
        for(auto curAppt : orig)
        {
            if (solution[i++] == 'C')
            {
                cSet.emplace(curAppt);
            }
            else
            {
                jSet.emplace(curAppt);
            }
        }

        if ( isScheduleValid(cSet) && isScheduleValid(jSet))
        {
            return getOrigOrderString(cSet, jSet, origOrder);
        }

        addJByRef(solution);
    }

    return "IMPOSSIBLE";
}

void solveFirstApproach(int probNumber, std::set<Appt> apptList, std::vector<Appt> apptListOriginalOrder)
{
    std::set<Appt> cameronList;
    std::set<Appt> jamieList;

    for(auto curAppt : apptList)
    {
        if (doesApptConflictWithSchedule(curAppt, cameronList))
        {
            // This one will have to be taken by jamie!
            DEBUG << "Appt " << apptToString(curAppt) << " doesn't work for cameron" << std::endl;

            if (doesApptConflictWithSchedule(curAppt, jamieList))
            {
                // Impossible schedule!
                DEBUG << "Appt " << apptToString(curAppt) << " doesn't work for jamie either" << std::endl;
                std::cout << "Case #" << probNumber << ": IMPOSSIBLE" << std::endl;
                return;
            }
            else
            {
                DEBUG << "Appt " << apptToString(curAppt) << " added to jamie's schedule" << std::endl;
                jamieList.emplace(curAppt);
            }
        }
        else
        {
            DEBUG << "Appt " << apptToString(curAppt) << " added to cameron's schedule" << std::endl;
            cameronList.emplace(curAppt);
        }
    }

    // If we got this, far, we have a good schedule

    // Iterate through the original schedule and print who got each appt
    std::string assList;
    for(auto curAppt : apptListOriginalOrder)
    {
        if (isApptInSchedule(curAppt, cameronList))
        {
            assList += "C";
        }
        else
        {
            assList += "J";
        }
    }

    std::cout << "Case #" << probNumber << ": " << assList << std::endl;
}

void solveProblem(int probNumber)
{
    int numAppts;
    std::cin >> numAppts;

    DEBUG << "Working on problem " << probNumber << " with " << numAppts << " appointments" << std::endl;

    std::set<Appt> apptList;
    std::vector<Appt> apptListOriginalOrder;

    for(int i = 0; i < numAppts; i++)
    {
        Appt curAppt = readAppt();
        apptList.emplace(curAppt);
        apptListOriginalOrder.push_back(curAppt);
    }

    printApptList(apptList);

    // solveFirstApproach(probNumber, apptList, apptListOriginalOrder);


    std::string solution = findRandomSolution(apptList, apptListOriginalOrder);

    // reorder solution

    std::cout << "Case #" << probNumber << ": " << solution << std::endl;
}


int main(int argc, char** argv)
{
    int numProblems;
    std::cin >> numProblems;

    for(int i = 0; i < numProblems; i++)
    {
        solveProblem(i+1);
    }

    return 0;
}
