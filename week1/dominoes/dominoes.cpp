#include <iostream>
#include <vector>

typedef std::pair<int, int> domino;
void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<domino> dominos;
    dominos.reserve(n);
    for(int i = 0; i < n; ++i){
        int hi = 0; std::cin >> hi;
        dominos[i].first = i;    // start of the interval
        dominos[i].second = i + hi;    // end of the interval
    }
    int maxEnd = dominos[0].second;
    bool found = false;
    int idx;
    for(idx = 1; idx < n; ++idx){
        int si = dominos[idx].first;
        int ei = dominos[idx].second;
        if(si >= maxEnd){
            found = true;
            break;
        }
        if(ei > maxEnd){
            maxEnd = ei;
        }
    }
    if(found){    // find the index of the first tile that does not fall down
        std::cout << idx << std::endl;
    }
    else{    // all tiles fall down
        std::cout << n << std::endl;
    }

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
