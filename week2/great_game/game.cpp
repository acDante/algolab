#include <iostream>
#include <vector>

typedef std::vector<std::vector<int> > AdjacencyList;
std::vector<int> min_memo;
std::vector<int> max_memo;
AdjacencyList neighbors;

int minStep(int i, int tar);
int maxStep(int i, int tar);

// minimal possible # of steps starting from position i
int minStep(int i, int tar){
    if(i == tar)
        min_memo[i] = 0;
    else if(min_memo[i] != -1)
        return min_memo[i];
    else{
        int numStep = INT32_MAX;
        for(int ni : neighbors[i]){
            numStep = std::min(maxStep(ni, tar) + 1, numStep);
        }
        min_memo[i] = numStep;
    }
    return min_memo[i];
}

// maximal possible # of steps starting from position i
int maxStep(int i, int tar){
    if(i == tar)
        max_memo[i] = 0;
    else if(max_memo[i] != -1)
        return max_memo[i];
    else{
        int numStep = 0;
        for(int ni : neighbors[i]){
            numStep = std::max(minStep(ni, tar) + 1, numStep);
        }
        max_memo[i] = numStep;
    }
    return max_memo[i];
}

void testcase(){
    int n, m;  // n: # of positions, m: # of transitions
    std::cin >> n >> m;
    int r, b; // r: start position of red meeple, b: start position of black meeple
    std::cin >> r >> b;
    neighbors.clear();
    neighbors.resize(n + 1);

    for(int i = 0; i < m; ++i){
        int u, v;
        std::cin >> u >> v;
        neighbors[u].push_back(v);
    }
    
    min_memo.clear(); 
    min_memo = std::vector<int>(n + 1, -1);
    max_memo.clear(); 
    max_memo = std::vector<int>(n + 1, -1);
    int sherlock = minStep(r, n);
    int moriaty = minStep(b, n);
    // std::cout << minStep_b << " " << minStep_r << std::endl;
    if(sherlock < moriaty)
        std::cout << 0;
    else if(sherlock == moriaty){
        if(sherlock % 2 == 0)
            std::cout << 1;
        else
            std::cout << 0;
    }
    else
        std::cout << 1;
    std::cout << std::endl;
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