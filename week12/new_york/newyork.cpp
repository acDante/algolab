/**
 * 50 pts solution
 * Algorithm: sliding window on Tree, in Bottom-up fashion (starting from leaves)
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

void testcase(){
    int n, m, k;
    std::cin >> n >> m >> k;
    std::vector<int> temperatures(n);
    for(int i = 0; i < n; ++i){
        std::cin >> temperatures[i];
    }

    std::vector<std::vector<int> > children(n, std::vector<int>());
    std::vector<int> parent(n, -1);   // store parent node of each vertex
    for(int i = 0;  i < n - 1; ++i){
        int u, v;
        std::cin >> u >> v;
        children[u].push_back(v);
        parent[v] = u;
    }

    std::vector<int> leaves;   // store all the leaf nodes
    std::set<int> results;     // store all the possible starting nodes
    for(int i = 0; i < n; ++i){
        if(children[i].size() == 0){
            leaves.push_back(i);
        }
    }

    std::vector<bool> visited(n, false);
    for(int leaf : leaves){
        int len = 1, tail = leaf; int head = tail;
        std::multiset<int> tempList;   // store the temperature along a path of length m
        tempList.insert(temperatures[tail]);        
        visited[tail] = true;
        while(len < m){
            if(parent[head] == -1){
                break;
            }
            head = parent[head];
            tempList.insert(temperatures[head]);
            len++;
        }

        if(len < m){
            continue;
        }
        else{
            // check if current path is feasible
            int minTemp = *tempList.begin();
            int maxTemp = *tempList.rbegin();

            if(maxTemp - minTemp <= k){
                results.insert(head);
            }
            // apply sliding window from current path until head reaches the root node
            while(parent[head] != -1){
                head = parent[head];
                tempList.insert(temperatures[head]);

                auto itr = tempList.find(temperatures[tail]);
                tempList.erase(itr);
                tail = parent[tail];
                if(visited[tail]) 
                    break;
                visited[tail] = true;
                minTemp = *tempList.begin();
                maxTemp = *tempList.rbegin();
                if(maxTemp - minTemp <= k){
                    results.insert(head);
                }
            }
        }
    }
    if(results.size() > 0){
        for(int node : results){
            std::cout << node << " ";
        }
        std::cout << "\n";
    }
    else{
        std::cout << "Abort mission" << "\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
