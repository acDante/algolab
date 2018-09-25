#include <iostream>

void testcase(){
    int n = 0, sum = 0;
    std::cin >> n;
    for(int i = 0; i < n; ++i){
        int ai;
        std::cin >> ai;
        sum += ai;
    }
    std::cout << sum << std::endl;
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
