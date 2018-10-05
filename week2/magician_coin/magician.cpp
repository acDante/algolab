#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

typedef std::vector<std::vector<double> >  Table;

Table memo;
std::vector<double> prob;
int n, m, k;

// DP(i, j) := the maximal probability to reach money m at n-th day ,if starting from i-th day, with money j
double maxProb(int i, int j){
    if(i == n && j == m)
        memo[i][j] = 1;
    else if(i == n && j < m)
        memo[i][j] = 0;
    if(memo[i][j] != -1){
        // std::cout << "maxProb(" << i << ", " << j << ")= " << memo[i][j] << std::endl;
        return memo[i][j];
    }
    else{
        double max_prob = 0;
        for(int bi = 0; bi <= j; ++bi){
            double cur_prob = maxProb(i + 1, std::min(j + bi, m)) * prob[i]
                              + maxProb(i + 1, j - bi) * (1 - prob[i]);
            // trick: 这里用std::min(j + bi, m)以压缩状态空间。 因为对于任何j > m, DP(i, j) = DP(i, m) = 1
            max_prob = std::max(max_prob, cur_prob);
        }
        memo[i][j] = max_prob;
    }
    // std::cout << "maxProb(" << i << ", " << j << ")= " << memo[i][j] << std::endl;
    return memo[i][j];
}

void print_memo(std::vector<std::vector<double> > p){
    // std::cout << p. << std::endl;
    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= m; ++j){
            std::cout << p[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_prob(std::vector<double> prob){
    for(double pi : prob){
        std::cout << pi << " ";
    }
    std::cout << std::endl;
}

void testcase(){
    // int n, k, m;
    std::cin >> n >> k >> m;
    prob.clear(); 
    prob.resize(n); prob[0] = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> prob[i];
    }

    memo.clear();
    memo.resize(n + 1);

    for(int i = 0; i <= n; ++i){
        memo[i].resize(m + 1);
        memo[i] = std::vector<double>(m + 1, -1);
    }
    std::cout << std::fixed << std::setprecision(5);
    std::cout << maxProb(0, k) << std::endl;
    // print_memo(memo);
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
