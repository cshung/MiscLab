#include "hungarian.h"
#include <iostream>
using namespace std;

int hungarian_test()
{
    int row1[] = {56, 87, 47, 87};
    int row2[] = {16, 31, 28, 26};
    int row3[] = {37, 40, 78, 72};
    int row4[] = {66, 36, 60, 55};
    vector<vector<int>> costs;
    costs.push_back(vector<int>(row1, row1 + 4));
    costs.push_back(vector<int>(row2, row2 + 4));
    costs.push_back(vector<int>(row3, row3 + 4));
    costs.push_back(vector<int>(row4, row4 + 4));
    vector<pair<int, int>> matching = hungarian(costs);
    for (auto edge : matching)
    {
        cout << edge.first << ", " << edge.second << " has cost " << costs[edge.first][edge.second] << endl;
    }
    return 0;
}
