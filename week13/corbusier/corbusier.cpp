// DP(i, mod): if it is possible to use disks[i:] to achieve sum_length % k = mod
// Iterative implementation
#include <iostream>
#include <vector>
#include <climits>
#include <cassert>

// int n, i, k;
// std::vector<std::vector<int> > memo;

// dp(i, mod) = whether it is possible to use disks[i:] to achieve that sum_if_length % k = mod
// int achievable(int idx, int mod, std::vector<int>& h){
//     if(idx == n - 1){
//         memo[idx][mod] = (h[idx] % k == mod);
//         return memo[idx][mod];
//     }
//     if(memo[idx][mod] != -1)
//         return memo[idx][mod];
//     else{
//         if(h[idx] % k == mod)   // can achieve mod using current disk only
//             memo[idx][mod] = 1;
//         else{
//             int residual = mod - h[idx] % k;
//             if(residual < 0)
//                 residual += k;
//             memo[idx][mod] = achievable(i + 1, mod, h) || achievable(i + 1, residual, h);
//         }
//         return memo[idx][mod];
//     }
//     // std::cout << "memo[" << idx << ", " << mod << "] = " << memo[idx][mod] << "\n";
// }

void testcase(){
    int n, i, k;
    std::cin >> n >> i >> k;
    std::vector<int> h(n);
    for(int i = 0; i < n; ++i){
        std::cin >> h[i];
    }
    // memo.clear();
    // memo = std::vector<std::vector<int> >(n, std::vector<int>(k, -1));
    std::vector<std::vector<bool> > achievable(n, std::vector<bool>(k));

    // Initialize the DP table: check if it is possible to achieve modulo mod with element h[idx]
    for(int idx = n - 1; idx >= 0; idx--){
        for(int mod = 0; mod < k; mod++){
            achievable[idx][mod] = (h[idx] % k == mod);
        }
    }
    // std::cout << "Finish initialization \n";

    for(int idx = n - 2; idx >= 0; idx--){
        for(int mod = 0; mod < k; mod++){
            int residual = mod - h[idx] % k;
            if(residual < 0)
                residual += k;
            achievable[idx][mod] = achievable[idx][mod] || achievable[idx + 1][mod] || achievable[idx + 1][residual];
        }
    }
    bool possible = achievable[0][i];
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
