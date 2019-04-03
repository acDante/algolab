// Dynamic Programming solution to bonus level problem
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <climits>

std::map<std::vector<int>, int> memo;
std::vector<std::vector<int> > bonus;

bool isValid(int x, int n, int k){
    int y = k - x;
    return ((x >= 0 && x < n) && (y >= 0 && y < n));
}

int maxReward(int k, int x1, int x2, int n){
    std::vector<int> index = {k, x1, x2};
    if(!isValid(x1, n, k) || !isValid(x2, n, k)){
        memo[index] = -1;
        return memo[index];
    }

    if(memo.find(index) == memo.end()){
        if(k == 0 && x1 == 0 && x2 == 0)
            memo[index] = bonus[0][0];
        else {
            int cur_reward = 0;
            if(isValid(x1, n, k) && isValid(x2, n, k)){
                if(x1 == x2)
                    cur_reward = bonus[x1][k - x1];
                else
                    cur_reward = bonus[x1][k - x1] + bonus[x2][k - x2];
            }
            std::vector<int> dx = {0, -1};
            int max_reward = - INT_MAX;
            for(int i = 0; i < 2; ++i){
                for(int j = 0; j < 2; ++j){
                    if(isValid(x1 + dx[i], n, k - 1) && isValid(x2 + dx[j], n, k - 1))
                       max_reward = std::max(max_reward, maxReward(k - 1, x1 + dx[i], x2 + dx[j], n) + cur_reward);
                }
            }
            memo[index] = max_reward;
        }
    }
    std::cout << index[0] << " " << index[1] << " " << index[2] << " : " << memo[index] << "\n";
    return memo[index];
}

void testcase(){
    int n;
    std::cin >> n;

    bonus = std::vector< std::vector<int> >(n, std::vector<int>(n));
    memo.clear();

    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> bonus[i][j];
        }
    }
    std::cout << maxReward(2 * n - 2, n - 1, n - 1, n) << std::endl;
}

int main(){
    int T = 0;
    std::cin >> T;
    while(T--){
        testcase();
    }
    return 0;
}
