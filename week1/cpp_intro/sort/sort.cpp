#include<iostream>
#include<algorithm>
#include<vector>

void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<int> v;
    v.reserve(n);
    for(int i = 0; i < n; ++i){
        int ai = 0;
        std::cin >> ai;
        v.push_back(ai);
    }
    int x = 0;
    std::cin >> x;
    if(x == 0){
        std::sort(v.begin(), v.end());
    }
    else if(x == 1){
        std::sort(v.begin(), v.end(), std::greater<int>());
    }
    for (int vi : v){
        std::cout << vi << " ";
    }
    std::cout << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
