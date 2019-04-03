// brute force: backtracking sotlution (20 pts)
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<std::vector<int> > Table;
typedef std::vector<std::vector<long> > LTable;

LTable score;
Table neighbors;
Table memo;

bool reachable(int u, int remStep, long remScore, int maxStep, int & minStep){
    if(remScore <=0 && remStep >= 0){
        minStep = std::min(minStep, maxStep - remStep);
        return true;
    }
    if(remStep == 0 && remScore > 0)
        return false;
    if(neighbors[u].size() == 0)
        return reachable(0, remStep, remScore, maxStep, minStep);
    
    bool found = false;
    for(int v : neighbors[u]){
        if (reachable(v, remStep - 1, remScore - score[u][v], maxStep, minStep))
            found = true;
    }
    return found;
}

void testcase(){
    int n, m, k; long x;
    std::cin >> n >> m >> x >> k;
    // std::cout << n << " " << m << " " << x << " " << k << "\n";
    
    score =  LTable(n, std::vector<long>(n, -1));
    neighbors =  Table(n, std::vector<int>());
    memo = Table(n, std::vector<int>(k + 1, -1));

    for(int i = 0; i < m; ++i){
        int u, v; long p;
        std::cin >> u >> v >> p;
        score[u][v] = std::max(score[u][v], p);
        neighbors[u].push_back(v);
    }

    if(neighbors[0].empty()){
        std::cout << "Impossible\n";
    }

    else{
        int minStep = INT_MAX;
        if(reachable(0, k, x, k, minStep))
            std::cout << minStep << "\n";
        else
            std::cout << "Impossible\n";
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
