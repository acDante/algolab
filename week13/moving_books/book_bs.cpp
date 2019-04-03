/**
 * Binary Search solution  O(NlogN)
 * The idea is to check if it is possible to finish within a fixed number of rounds,
 * and find the minimal possible rounds by binary search
 */

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <climits>
#include <cassert>
#include <algorithm>

bool possible_to_finish(int r, std::vector<int>& s, std::vector<int>& w){
    int i = 0, j = 0;
    // Explanation: consider assigning the r heaviest boxes to each person
    while(i < s.size() && j < w.size()){
        if(w[j] <= s[i]){
            i = i + 1;
            j = j + r;
        }
        else
            return false;
    }
    return (j >= w.size());
}

void testcase(){
    int n, m;    // n: # of friends, m: # of boxess
    std::cin >> n >> m;
    std::vector<int> s(n);
    std::vector<int> w(m);
    for(int i = 0; i < n; ++i){
        std::cin >> s[i];
    }
    for(int i = 0; i < m; ++i){
        std::cin >> w[i];
    }
    std::sort(s.begin(), s.end(), std::greater<int>());
    std::sort(w.begin(), w.end(), std::greater<int>());

    if(*s.begin() < *w.begin()){
        std::cout << "impossible\n";
        return;
    }
    // Binary search the optimal r
    int low = 0, high = w.size();
    int num_round = 0;
    while(low <= high){
        int mid = (low + high) / 2;
        if(possible_to_finish(mid, s, w)){
            num_round = mid;
            high = mid - 1;
        }
        else
            low = mid + 1;
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
