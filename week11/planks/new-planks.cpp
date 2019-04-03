// 100 pt solution using std::vector
// Be careful with "auto", it may slow down your implementation!!!! 
// Always use index to access elements, don't be lazy

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <numeric>

typedef std::vector<int> VI;
typedef std::vector<std::vector<int> > VII;

void findSubset(int low, int high, const VI& l, VII& subsets, VII& F){
    if(low == high){
        std::vector<int> tuple(4, 0);
        for(int i = 0; i < 4; ++i){
            tuple[i] = std::accumulate(subsets[i].begin(), subsets[i].end(), 0);
        }
        F.push_back(tuple);
        return;
    }
    for(int i = 0; i < 4; ++i){
        subsets[i].push_back(l[low]);
        findSubset(low + 1, high, l, subsets, F);
        subsets[i].pop_back();
    }
}

void testcase(){
    int n;
    std::cin >> n;
    std::vector<int> l(n);
    int sum = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> l[i];
        sum += l[i];
    }

    if(sum % 4 != 0){
        std::cout << 0 << "\n";
        return;
    }

    VII subsets1(4, std::vector<int>()), subsets2(4, std::vector<int>());
    VII F1, F2;
    findSubset(0, n / 2, l, subsets1, F1);
    findSubset(n / 2, n, l, subsets2, F2);
    std::sort(F2.begin(), F2.end());

    long long result = 0;
    for(int idx = 0; idx < F1.size(); ++idx){
        std::vector<int> complement(4, 0);
        for(int i = 0; i < 4; ++i){
            complement[i] = sum / 4 - F1[idx][i];
        }
        auto bound = std::equal_range(F2.begin(), F2.end(), complement);
        long long count = std::distance(bound.first, bound.second);
        result += count;
    }
    std::cout << result / 24 << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i)
        testcase();
    return 0;
}
