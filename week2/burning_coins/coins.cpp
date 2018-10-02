#include <iostream>
#include <vector>
#include <map>

std::vector<int> v;
std::vector<int> partialSum;
std::vector<std::vector<int> > memo;
//std::map<std::pair<int, int>, int> memo;

void print(std::vector<int> v){
    for(int vi: v){
        std::cout << vi << " ";
    }
    std::cout << std::endl;
}

// compute sum:= v[i:j] both included 
int sum(int i, int j){
    if (i == 0)
        return partialSum[j];
    else
        return partialSum[j] - partialSum[i - 1];
}

// Use dynamic programming to compute maximal possible value
int maxValue(int i, int j){
    if(i == j)
        return v[i];

    if(memo[i][j] != -1){
        return memo[i][j];
    }
    int maxVal = std::max(v[i] + sum(i + 1, j) - maxValue(i + 1, j),
                          v[j] + sum(i, j - 1) - maxValue(i, j - 1));
    memo[i][j] = maxVal;
    return memo[i][j];
}
 
void testcase(){
    int n;
    std::cin >> n;

    v.clear();
    partialSum.clear();
    memo.clear(); memo.resize(n);

    int sum = 0;
    for(int i = 0; i < n; ++i){
        int vi;
        std::cin >> vi;
        v.push_back(vi);
        
        sum += vi;
        partialSum.push_back(sum);
    }
    
    for(int i = 0; i < n; ++ i){
        memo[i] = std::vector<int>(n, -1);
    }

    std::cout << maxValue(0, n - 1) << std::endl;
    // print_memo();
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
