// 20 pts solution with std::map....really slow 

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

typedef std::vector<int> VI;
typedef std::vector<std::vector<int> > VII;

void findSubset(int low, int high, VI& l, VII& subsets, std::map<VI, int>& count){
    if(low == high){
        std::vector<int> subset_sum(4, 0);
        for(int i = 0; i < 4; ++i){
            subset_sum[i] = std::accumulate(subsets[i].begin(), subsets[i].end(), 0);
        }
        if(count.find(subset_sum) != count.end())
            count[subset_sum]++;
        else
            count[subset_sum] = 1;
        return;
    }
    for(int i = 0; i < 4; ++i){
        subsets[i].push_back(l[low]);
        findSubset(low + 1, high, l, subsets, count);
        subsets[i].pop_back();
    }
}

void testcase(){
    int n;
    std::cin >> n;
    std::vector<int> l(n);
    int target = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> l[i];
        target += l[i];
    }
    if(target % 4 != 0){
        std::cout << 0 << "\n";
        return;
    }
    target /= 4;

    VII subsets_left(4, std::vector<int>());
    VII subsets_right(4, std::vector<int>());
    std::map<VI, int> count_left, count_right;
    
    findSubset(0, n/2, l, subsets_left, count_left);
    findSubset(n/2, n, l, subsets_right, count_right);

    int total_count = 0;
    for(auto record : count_left){
        std::vector<int> key(4, 0);
        for(int i = 0; i < 4; ++i){
            key[i] = target - record.first[i];
        }
        if(count_right.find(key) != count_right.end()){
            total_count += record.second * count_right[key];
        }
    }
    std::cout << total_count / 24 << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i)
        testcase();
    return 0;
}