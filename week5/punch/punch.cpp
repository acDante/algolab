// Store a tuple <cost, diversity, use> at each DP state
// First minimize the cost, if the cost from two directions are the same, 
// maximize the diversity and set the corresponding flag which indicates if i-th beverage is used

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct State{
    int cost;
    int diversity;
    bool use;    // indicate if the i-th beverage is used
};
void testcase(){
    int n, k;
    std::cin >> n >> k;
    std::vector<int> cost(n);
    std::vector<int> volume(n);
    for(int i = 0; i < n; ++i){
        std::cin >> cost[i] >> volume[i];
    }
    std::vector<std::vector<State> > dp(n, std::vector<State>(k + 1));
    // Initialization
    // Case 1: use the first i beverages to achieve 0 iter
    for(int i = 0; i < n; ++i){
        dp[i][0].cost = 0;
        dp[i][0].diversity = 0;
        dp[i][0].use = false;
    }
    // Case 2: use the first beverage to achieve at least v iter
    for(int v = 0; v <= k; ++v){
        dp[0][v].cost = ceil(double(v) / volume[0]) * cost[0];
        dp[0][v].use = (dp[0][v].cost > 0) ? true : false;
        dp[0][v].diversity = (dp[0][v].use == true) ? 1 : 0;
    }   
   for(int i = 1; i < n; ++i){
       for(int v = 1; v <= k; ++v){
           // cost and diversity if use i-th beverage
           int index = std::max(v - volume[i], 0);
           int cost1 = dp[i][index].cost + cost[i];
           int diversity = (dp[i][index].use == true) ? dp[i][index].diversity : dp[i][index].diversity + 1;
           if(cost1 < dp[i - 1][v].cost){
               dp[i][v].cost = cost1;
               dp[i][v].diversity = diversity;
               dp[i][v].use = true;
           }
           else if(cost1 > dp[i - 1][v].cost){
               dp[i][v].cost = dp[i - 1][v].cost;
               dp[i][v].diversity = dp[i - 1][v].diversity;
               dp[i][v].use = false;
           }
           else{    // cost == dp[i - 1][v].cost
               if(diversity > dp[i - 1][v].diversity){
                   dp[i][v].cost = cost1;
                   dp[i][v].diversity = diversity;
                   dp[i][v].use = true;
               }
               else{   // diversity <= dp[i - 1][v].diversity
                   dp[i][v].cost = dp[i - 1][v].cost;
                   dp[i][v].diversity = dp[i - 1][v].diversity;
                   dp[i][v].use = false;
               }
           }
       }
   }
   std::cout << dp[n - 1][k].cost << " " << dp[n - 1][k].diversity << "\n";

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
