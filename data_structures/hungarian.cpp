#include "hungarian.h"
#include <queue>
using namespace std;

// Implementation of the Hungarian algorithm for finding a minimum cost perfect matching
// The implementation is based on the description on https://en.wikipedia.org/wiki/Hungarian_algorithm
vector<pair<int,int>> hungarian(const vector<vector<int>>& costs)
{
    // The problem of finding a minimum cost assignment is the same as
    // finding the minimum cost perfect matching between the rows (workers) and the columns (jobs)
    int n = (int)costs.size();
    int m = (int)costs[0].size();
    
    // At all times, the algorithm maintains an integer named potential for all nodes.
    // The algorithm make sure potential(u) + potential(v) <= cost[u,v]
    // The nodes that represents the row are [0,n) and the nodes that represents the columns are [n, m + n)
    // Potential values are initialized to 0.
    vector<int> potential;
    potential.resize(m + n);
    for (int i = 0; i < m + n; i++)
    {
        potential[i] = 0;
    }
    
    // We maintain a directed graph, the edges of the graph is tight (i.e. potential(u) + potential(v) = cost[u,v]).
    // The edges from [n, m + n) back to [0, n) has to form a matching. (i.e. these edges does not share source/target)
    vector<vector<bool>> direction;
    direction.resize(n);
    for (int i = 0; i < n; i++)
    {
        direction[i].resize(m);
        for (int j = 0; j < m; j++)
        {
            direction[i][j] = true;
        }
    }
    
    // If a node is involved in the matching, the matched flag will be true.
    vector<bool> matched;
    matched.resize(m + n);
    for (int i = 0; i < m + n; i++)
    {
        matched[i] = false;
    }
    
    // We will do a BFS to find paths. The parent array store the parent node used to discover a node.
    vector<int> parent;
    parent.resize(m + n);

    while (true)
    {
        queue<int> bfs;
        for (int i = 0; i < m + n; i++)
        {
            // Parent == -1 means it is not enqueued yet, so there is no parent
            parent[i] = -1;
        }
        
        for (int i = 0; i < n; i++)
        {
            if (!matched[i])
            {
                bfs.push(i);
                // Parent == -2 means it is enqueued because it is a not matched source node.
                // It should NOT be enqueued again
                parent[i] = -2;
            }
        }
        
        // This is just a standard BFS, note that the cost and direction array must be read from row to column
        while (bfs.size() > 0)
        {
            int visiting = bfs.front();
            bfs.pop();
            if (visiting < n)
            {
                for (int neighbor = n; neighbor < m + n; neighbor++)
                {
                    bool tight = costs[visiting][neighbor - n] - potential[visiting] - potential[neighbor] == 0;
                    if (direction[visiting][neighbor - n] && tight)
                    {
                        if (parent[neighbor] == -1)
                        {
                            bfs.push(neighbor);
                            parent[neighbor] = visiting;
                        }
                    }
                }
            }
            else
            {
                for (int neighbor = 0; neighbor < n; neighbor++)
                {
                    bool tight = costs[neighbor][visiting - n] - potential[visiting] - potential[neighbor] == 0;
                    if (!direction[neighbor][visiting - n] && tight)
                    {
                        if (parent[neighbor] == -1)
                        {
                            bfs.push(neighbor);
                            parent[neighbor] = visiting;
                        }
                    }
                }
            }
        }
        
        bool found = false;
        for (int i = n; i < m + n; i++)
        {
            if (!matched[i] && parent[i] != -1)
            {
                // If there is a way to reach from a source unmatched node to a target
                // unmatched node, then we have an augmenting path, using the parent
                // pointer chain, we can reverse all the paths to increasing the size
                // of the matching by 1.
                found = true;
                int current = i;
                while (true)
                {
                    if (parent[current] == -2)
                    {
                        matched[i] = true;
                        matched[current] = true;
                        break;
                    }
                    int small = min(current, parent[current]);
                    int large = max(current, parent[current]);
                    direction[small][large - n] = !direction[small][large - n];
                    current = parent[current];
                }
                break;
            }
        }
        
        if (!found)
        {
            // If we cannot find an augmenting path, we will modify the potential
            // so that we have more nodes reachable from the unmatched sources
            int delta = -1;
            for (int i = 0; i < n; i++)
            {
                if (parent[i] != -1)
                {
                    for (int j = n; j < m + n; j++)
                    {
                        if (parent[j] == -1)
                        {
                            // Make sure the delta does not violate the constraint
                            // between each pair of reachable source/unreachable target
                            int margin = costs[i][j - n] - potential[i] - potential[j];
                            if (delta == -1 || margin < delta)
                            {
                                delta = margin;
                            }
                        }
                    }
                }
            }
            if (delta == -1)
            {
                break;
            }
            for (int i = 0; i < n; i++)
            {
                if (parent[i] != -1)
                {
                    // For each reachable source, the potential is increased by delta
                    potential[i] += delta;
                }
            }
            for (int i = n; i < m + n; i++)
            {
                if (parent[i] != -1)
                {
                    // For each reachable target, the potential is decreased by delta
                    potential[i] -= delta;
                }
            }
            // After the manipulation:
            // The potential is feasible for reachable source, reachable target because the sum didn't change.
            // The potential is feasible for reachable source, unreachable target because the increase is checked earlier.
            // The potential is feasible for unreachable source, reachable target because the sum decrease.
            // The potential is feasible for unreachable source, unreachable target because the sum didn't change.
        }
    }
    vector<pair<int,int>> answer;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (!direction[i][j])
            {
                answer.push_back(make_pair(i, j));
            }
        }
    }
    return answer;
}

