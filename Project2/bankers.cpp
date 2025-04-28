#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
    int n, m; // number of processes, number of resources
    ifstream input("input.txt");

    input >> n >> m;

    vector<int> totalResources(m);
    for (int i = 0; i < m; i++)
        input >> totalResources[i];

    vector<int> available(m);
    for (int i = 0; i < m; i++)
        input >> available[i];

    vector<vector<int>> allocation(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            input >> allocation[i][j];

    vector<vector<int>> maxNeed(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            input >> maxNeed[i][j];

    input.close();

    // Calculate Need matrix
    vector<vector<int>> need(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxNeed[i][j] - allocation[i][j];

    vector<bool> finished(n, false);
    vector<int> safeSequence;
    int count = 0;

    while (count < n)
    {
        bool found = false;
        for (int i = 0; i < n; i++)
        {
            if (!finished[i])
            {
                bool canAllocate = true;
                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > available[j])
                    {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate)
                {
                    for (int j = 0; j < m; j++)
                        available[j] += allocation[i][j];
                    safeSequence.push_back(i);
                    finished[i] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found)
        {
            cout << "The system is not in a safe state." << endl;
            return 0;
        }
    }

    cout << "The system is in a safe state." << endl;
    cout << "Safe sequence is: ";
    for (int i = 0; i < n; i++)
        cout << "P" << safeSequence[i] << (i == n - 1 ? "\n" : " -> ");

    return 0;
}
