#include <iostream>
#include <vector>
#include <cmath>

// void print(std::vector<std::vector<double> > p){
//     std::cout << p. << std::endl;
//     for(int i = 0; i < p.size(); ++ i){
//         for(int j = 0; j < p[i].size(); ++j){
//             std::cout << p[i][j] << "\t";
//         }
//         std::cout << std::endl;
//     }
//     std::cout << std::endl;
// }

void testcase(){
    int n, k, m;
    std::cin >> n >> k >> m;
    std::vector<double> prob;
    prob.reserve(n + 1); prob[0] = 0;
    for(int i = 1; i <= n; ++i){
        double pi = 0;
        std::cin >> pi;
        prob.push_back(pi);
    }
    std::vector<std::vector<double> > p;
    p.reserve(n + 1);
    for(int i = 0; i <= n; ++ i){
        p[i] = std::vector<double>(m + 1, 0);
        p[i][0] = 1;
    }

    // initialize the dp table
    for(int j = 1; j <= m; ++j){
        if(j <= k)
            p[0][j] = 1;
        else
            p[0][j] = 0;
    }

    for(int i = 1; i <= n; ++i){
        for(int j = 1; j <= m; ++ j){
            double max_prob = 0;
            for(int bi = ceil(j / 2.0); bi <= j; ++bi){
                max_prob = std::max(max_prob, p[i - 1][bi] * prob[i]);
            }
            max_prob = std::max(max_prob, p[i - 1][j]);
            p[i][j] = max_prob;
            std::cout << p[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    // print(p);
    std::cout << p[n][m] << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
