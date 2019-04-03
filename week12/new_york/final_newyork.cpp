#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <set>
#include <algorithm>

int n, m, k;   // n: # of vertices, m: path length, k: risk bound
std::vector<long> temperature;
std::vector<std::vector<int> > children;
// std::vector<bool> visited;

void print(std::vector<int> vec){
    for(int v : vec)
        std::cout << v << " ";
    std::cout << "\n";
}
void print_ms(std::multiset<int> ms){
    for(int v : ms)
        std::cout << v << " ";
    std::cout << "\n"; 
}

void dfs(int u, std::vector<bool>& visited, std::vector<int>& path, 
            std::multiset<long>& tempList, std::set<int>& results){
    // print(path);
    visited[u] = true;
    if(tempList.size() == m){
        long minTemp = *tempList.begin();
        long maxTemp = *tempList.rbegin();
        if(maxTemp - minTemp <= k){
            results.insert(path[path.size() - m]);
        }
    }
    // print_ms(tempList);
    for(int v : children[u]){
        if(!visited[v]){
            path.push_back(v);
            tempList.insert(temperature[v]);
            long temp = -1;
            if(tempList.size() > m){
                int head = path[path.size() - m - 1];
                temp = temperature[head];
                auto itr = tempList.find(temperature[head]);
                if(itr != tempList.end())
                    tempList.erase(itr);
            }

            dfs(v, visited, path, tempList, results);
            
            auto itr = tempList.find(temperature[v]);
            if(itr != tempList.end())
                tempList.erase(itr);
            path.pop_back();
            if(temp >= 0)
                tempList.insert(temp);
        }
    }
    return;
}

void testcase(){
    std::cin >> n >> m >> k;
    temperature = std::vector<long>(n, 0);
    for(int i = 0; i < n; ++i){
        std::cin >> temperature[i];
    }

    children = std::vector<std::vector<int> >(n, std::vector<int>());
    for(int i = 0; i < n - 1; ++i){
        int u, v;
        std::cin >> u >> v;
        children[u].push_back(v);
    }

    std::set<int> results;
    std::multiset<long> tempList;  tempList.insert(temperature[0]);
    std::vector<bool> visited(n, false);
    std::vector<int> path; path.push_back(0);

    // std::cout << "Arrive here \n";
    dfs(0, visited, path, tempList, results);
    // dfs(0, 1, visitList, tempList, results);

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
