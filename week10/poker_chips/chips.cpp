// 100 pt : 5 dimensional DP formulation  (improved with 
// Be careful with accessing data in std::map !! Avoid repeated acesss in recursion !!
// Another tip: try replacing std::map with std::vector to improve the solution
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <climits>
#include <cassert>
#include <boost/functional/hash.hpp>

template <typename Container> // we can make this generic for any container [1]
struct container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};

std::vector< std::vector<int> > chips;
std::unordered_map<std::vector<int>, int, container_hash<std::vector<int> > > memo;

void print(std::vector<int> & vec){
    for(int v : vec){
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void print_chips(std::vector<std::vector<int> >& vec){
    for(int i = 0; i < vec.size(); ++i){
        for(int j = 0; j < vec[i].size(); ++j)
            std::cout << vec[i][j] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

int maxScore (int n, const std::vector<int>& pos){
    std::vector<int> finalState(n, 0);
    if(pos == finalState)
        return 0;
    if(memo.find(pos) != memo.end())
        return memo[pos];
    
    std::set<int> colors;  // store all present colors
    std::map<int, std::vector<int> > cts;   // store the corresponding stack indices for each color
    for(int i = 0; i < n; ++i){
        if(pos[i] > 0){
            int cur_color = chips[i][ pos[i] ];
            colors.insert(cur_color);
            cts[cur_color].push_back(i);
        }
    }
    // print(colors);
    int result = 0;
    for(int c : colors){
        for(int mask = 1; mask < (1 << cts[c].size()); mask++){
            int count = 0;
            std::vector<int> new_pos = pos;
            for(int j = 0; j < cts[c].size(); j++){
                if(mask & (1 << j)){
                    count++;
                    new_pos[cts[c][j]] -= 1;
                }
            }
            // std::cout << "count = " << count << "\n";
            if(count > 1)
                result = std::max(result, maxScore(n, new_pos) + (1 << (count - 2) ) );
            else
                result = std::max(result, maxScore(n, new_pos));
        }
    }
    memo[pos] = result;
    return memo[pos];
}

void testcase(){
    int n;    // n: number of stacks
    std::cin >> n;

    // Initialization of global variables 
    chips.clear(); chips.resize(n);
    memo.clear();
    std::vector<int> m(n);    // size of each stack
    std::vector<int> lastPos(n);
    for(int i = 0; i < n; ++i){
        int mi; std::cin >> mi;
        chips[i].clear();
        chips[i].resize(mi + 1);
        m[i] = mi;

        lastPos[i] = mi;
    }

    for(int i = 0; i < n; ++i){
        for(int j = 1; j <= m[i]; ++j){
            std::cin >> chips[i][j];
        }
    }
    // print_chips(chips);
    std::cout << maxScore(n, lastPos) << "\n";
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
