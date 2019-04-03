// Use DP to compute maxScore(u, k) + binary search the minStep
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

typedef std::vector<std::vector<int> > Table;
typedef std::vector<std::vector<long> > LTable;

LTable score;
Table neighbors;
LTable memo;

long maxScore(int u, int remStep){
    if(remStep == 0)
        return 0;
    else if(memo[u][remStep] != -1)
        return memo[u][remStep];
    
    long res = 0;
    if(neighbors[u].empty())
        res = maxScore(0, remStep);
    else{
        for(int v : neighbors[u]){
            res = std::max(res, maxScore(v, remStep - 1) + score[u][v]);
        }
    }
    memo[u][remStep] = res;
    return memo[u][remStep];
}

bool BinSearch(int k, long x, int & minStep){
    int low = 1, high = k;
    bool found = false;
    while(low <= high){
        int mid = low + (high - low) / 2;
        if(maxScore(0, mid) >= x){
            minStep = std::min(minStep, mid);
            found = true;
            high = mid - 1;
        }
        else{
            low = mid + 1;
        }
    }
    return found;
}

void testcase(){
    int n, m, k; long x;
    std::cin >> n >> m >> x >> k;
    
    score.clear(); score =  LTable(n, std::vector<long>(n, -1));
    neighbors.clear(); neighbors =  Table(n, std::vector<int>());
    memo.clear(); memo = LTable(n, std::vector<long>(k + 1, -1));

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
        bool found = BinSearch(k, x, minStep);
        if(!found)
            std::cout << "Impossible\n";
        else
            std::cout << minStep << "\n";
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
