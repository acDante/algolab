#include <iostream>
#include <vector>

// DP(t, flag): until t-th group of lights, the minimal number of operations required to achieve a pattern
// boolean flag indicates that the resulting pattern is same wtih or opposite from expected pattern.
void testcase(){
    int n, k, x;
    std::cin >> n >> k >> x;
    std::vector<bool> pattern(k, 0);
    for(int i = 0; i < k; ++i){
        if(x & (1 << i))
            pattern[k - i - 1] = 1;
    }
    std::vector<bool> lights(n);
    for(int i = 0; i < n; ++i){
        int bit; std::cin >> bit;
        lights[i] = bit;
    }
    
    std::vector<std::vector<int> > minStep(n / k + 1, std::vector<int>(2));
    // initialize DP table
    int same = 0;   // number of same bits
    for(int j = 0; j < k; ++j){
        same += (lights[j] == pattern[j]);
    }
    int diff = k - same;   // number of different bits
    minStep[1][true] = std::min(same + 1, diff);
    minStep[1][false] = std::min(diff + 1, same);

    for(int t = 2; t <= n / k; ++t){
        int same = 0;   
        for(int j = 0; j < k; ++j)
            same += (lights[(t - 1) * k + j] == pattern[j]);
        int diff = k - same;
        // Two strategies: (1). change all the same bits one by one and negate all bits so far
        //                 (2). change all different bits one by one
        minStep[t][true] = std::min(minStep[t - 1][true] + diff, minStep[t - 1][false] + same + 1);
        minStep[t][false] = std::min(minStep[t - 1][false] + same, minStep[t - 1][true] + diff + 1);
    }
    std::cout << minStep[n / k][true] << "\n";
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
