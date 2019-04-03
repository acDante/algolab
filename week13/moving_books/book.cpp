// Greedy solution: multiset implementation  O(NlogN)
// - Erase, insert and find elements is done in O(logn)
// Greey strategy: in each round, assign each person the heaviest box he is able to lift 
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <climits>
#include <cassert>
#include <algorithm>

void testcase(){
    int n, m;    // n: # of friends, m: # of boxess
    std::cin >> n >> m;
    std::vector<int> s(n);
    std::vector<int> w(m);
    for(int i = 0; i < n; ++i){
        std::cin >> s[i];
    }

    /**
     * Attention here:
     * with std::greater<int> option, 
     * ms.lower_bound(): find the first element that are no greater than some value
     * ms.upper_bound(): find the first element less than some value
     */  
    std::multiset<int, std::greater<int> > ms;  
    for(int i = 0; i < m; ++i){
        std::cin >> w[i];
        ms.insert(w[i]);
    }
    std::sort(s.begin(), s.end(), std::greater<int>());

    if(*ms.begin() > *s.begin()){
        std::cout << "impossible\n";
        return;
    }
    int num_round = 0;
    while(!ms.empty()){
        for(int si : s){
            auto itr = ms.lower_bound(si);   
            // This equals to the largest element less than or equal to si since ms is sorted in decreasing order
            if(itr != ms.end())
                ms.erase(itr);
            else
                break;
        }
        num_round++;
    }
    std::cout << 3 * num_round - 1 << "\n";
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
