#include <iostream>
#include <vector>
#include <algorithm>

typedef std::pair<int, int> Jedi;
void testcase(){
    int n, m;
    std::cin >> n >> m;
    std::vector<Jedi> jedis(n);
    for(int i = 0; i < n; ++i){
        std::cin >> jedis[i].first >> jedis[i].second;
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}