#include <iostream>
#include <vector>

void testcase(){
    int n, k;
    std::cin >> n >> k;
    std::vector<int> students(n);
    for(int i = 0; i < n; ++i){
        std::cin >> students[i];
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