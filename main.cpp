#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <stdexcept>

using namespace std;

///Role stuff

enum Role{TOP, JG, MID, ADC, SUPP, COUNT};
const int ROLE_COUNT = static_cast<int>(Role::COUNT);

Role& operator++(Role& role) {
    role = static_cast<Role>( static_cast<int>(role)+1 );
    if(role == COUNT) throw overflow_error("Incremented last role.\n");
    return role;
}

Role operator++(Role& role, int) {
    Role tmp(role);
    ++role;
    return tmp;
}

ostream& operator<<(ostream& os, const Role& role)
{
    static std::map<Role, std::string> strings;
    if (strings.size() == 0){
        #define INSERT_ELEMENT(p) strings[p] = #p
            INSERT_ELEMENT(TOP);
            INSERT_ELEMENT(JG);
            INSERT_ELEMENT(MID);
            INSERT_ELEMENT(ADC);
            INSERT_ELEMENT(SUPP);
        #undef INSERT_ELEMENT
    }
    return os << strings[role];
}

///End of role stuff

///Global variables for storing the optimal solution and preferences
///otherwise would need to be parameters of every function call.
vector<vector<Role> > preferences;
vector<Role> optimalSolution(ROLE_COUNT, static_cast<Role>(0));

///CostCalculator class, you can define new criteria with polymorphism.
class CostCalculator {
public:
    virtual float calculate(vector<Role>);

    bool repeatedRoles(vector<Role> roles) {
        vector<bool> alreadyOccupied(ROLE_COUNT, false);
        for(auto eachPlayerAssignedRole : roles) {
            if(alreadyOccupied[static_cast<int>(eachPlayerAssignedRole)]) {
                    return true;
            }
            else alreadyOccupied[eachPlayerAssignedRole] = true;
        }
        return false;
    }
};

///An example of cost calculation.
class SimpleCostCalculator : public CostCalculator {
public:
    virtual float calculate(vector<Role> roleDistribution) {
        if(repeatedRoles(roleDistribution)) return 1000;

        float cost = 0;
        for(int i=0; i<preferences.size(); i++) {
            auto& currentPlayerPreferences = preferences[i];
            auto it = find(currentPlayerPreferences.begin(), currentPlayerPreferences.end(), roleDistribution[i]);
            int positionInPreference = it - currentPlayerPreferences.begin();
            cost += positionInPreference;
        }
        return cost;
    }
};

///Recursive function, tries every permutation and stores the one with least cost.
vector<Role> roleSolverRecursive(CostCalculator* costCalculator, vector<Role> curSolution = optimalSolution, int curPlayer = 0) {
    if(curPlayer < preferences.size()) {
        for(int i=0; i<ROLE_COUNT; i++){
            if(costCalculator->calculate(optimalSolution) > costCalculator->calculate(curSolution)) {
                optimalSolution = curSolution;
            }

            roleSolverRecursive(costCalculator, curSolution, curPlayer+1);
            if(i != ROLE_COUNT-1) curSolution[curPlayer]++;
            else break;
        }
    }
}

int main()
{
     preferences = {{TOP, JG, MID, ADC, SUPP},
                    {SUPP, TOP, JG, MID, ADC},
                    {ADC, SUPP, TOP, JG, MID},
                    {MID, ADC, SUPP, TOP, JG},
                    {TOP, JG, SUPP, MID, ADC}};

    SimpleCostCalculator costCalculator;
    roleSolverRecursive(&costCalculator);

    for(int i=0; i<preferences.size(); i++) {
        cout << "P" << i << '\t';
    }
    cout << endl;

    for(int curRole=0; curRole<ROLE_COUNT; curRole++) {
        for(int curPlayer=0; curPlayer<preferences.size(); curPlayer++) {
            cout << preferences[curPlayer][curRole] << '\t';
        }
        cout << endl;
    }

    cout << endl;

    for(auto assignedRole : optimalSolution) {
        cout << assignedRole << '\t';
    }
}
