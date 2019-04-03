// DP(i, mod): if it is possible to use disks[i:] to achieve sum_length % k = mod
// Recursive implementation (faster than iterative way in this case)
#include <iostream>
#include <vector>
#include <climits>
#include <cassert>

int n, i, k;
std::vector<std::vector<int> > memo;   // have to use vector<int> instead of vector<bool> to store -1

// dp(i, mod) = whether it is possible to use disks[i:] to achieve that sum_if_length % k = mod
int achievable(int idx, int mod, std::vector<int>& h){
    if(memo[idx][mod] != -1)
        return memo[idx][mod];
    if(idx == n - 1){
        memo[idx][mod] = (h[idx] % k == mod);
    }
    else{
        int residual = mod - h[idx] % k;
        if(residual < 0)
            residual += k;
        // Attention: make sure the index is correctly assigned ...!
        memo[idx][mod] = (h[idx] % k == mod) || achievable(idx + 1, mod, h) || achievable(idx + 1, residual, h);
    }
    return memo[idx][mod];
}

void testcase(){
    std::cin >> n >> i >> k;
    std::vector<int> h(n);
    for(int i = 0; i < n; ++i){
        std::cin >> h[i];
    }
    memo.clear(); memo.resize(n);
    for(int i = 0; i < n; ++i)
        memo[i] = std::vector<int>(k, -1);
    
    bool possible = achievable(0, i, h);
    if(possible)
        std::cout << "yes\n";
    else
        std::cout << "no\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
