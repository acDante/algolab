// Sliding window + dynamic programming 
#include <iostream>
#include <vector>

typedef std::pair<int, int>  PII;

// print the candidate intervals
void print(std::vector<PII> candidates){
    for (PII interval : candidates){
        std::cout << "(" << interval.first << ", " << interval.second << ")" << std::endl;
    }
    std::cout << std::endl;
}
void testcase(){
    int n, m, k;
    std::cin >> n >> m >> k;
    std::vector<int> defenders;
    defenders.reserve(n);
    for(int i = 0; i < n; ++i){
        int vi = 0;
        std::cin >> vi;
        defenders.push_back(vi);
    }

    // use sliding window to find all candidate intervals, store the interval length at val[j]
    std::vector<int> val(n, 0);
    int i = 0, j = 0;
    int cur_sum = defenders[0];
    while(j < n){
        if(cur_sum <= k){
            if(cur_sum == k){
                val[j] = j - i + 1;
            }
            j++;
            cur_sum += defenders[j];
        }
        else{
            cur_sum -= defenders[i];
            i++;
            if(i > j){
                j = i;
                cur_sum = defenders[j];
            }
        }
    }

    // Find the optimal defensive line, DP[i][j] := the maximal value if using i attackers to attack the first j defenders 
    std::vector<std::vector<int> >  memo;
    memo.resize(m + 1);
    for(int i = 0; i <= m; ++i){
        memo[i] = std::vector<int>(n, -1);  // -1 indicates that the value has not yet filled in
    }

    // Initialize the dp table with basic cases
    // Case 1: num of attackers = 0
    for(int j = 0; j < n; ++j){
        memo[0][j] = 0;
    }

    // Case 2: using i attackers to attack the first defender
    for(int i = 1; i <= m; ++i){
        memo[i][0] = val[0];
        // if(i == 1)
        //     memo[i][0] = val[0];
        // else
        //     memo[i][0] = - (n + 1);   // -(n + 1):= indicate there is no feasible solution, ensure the infeasible solution is negative number
    }

    for(int i = 1; i <= m; ++i){
        for(int j = 1; j < n; ++j){
            memo[i][j] = std::max(memo[i - 1][std::max(j - val[j], 0)] + val[j],  // Attention: Avoid index j-val[j] < 0 !!!
                                  memo[i][j - 1]);  
        }
    }
    
    /*  if adding a new attacker does not improve the maximal value,
     *  then we can conclude that some of the attackers are not assigned a valid defensive line 
     *  (since the reward is always positive)
     */
    // if(memo[m][n - 1] > 0)
    if(memo[m][n - 1] != memo[m - 1][n - 1])
        std::cout << memo[m][n - 1] << std::endl;
    else
        std::cout << "fail" << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}