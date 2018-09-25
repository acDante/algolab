// Method: first overwrite original value in the vector, then pop from end
#include <iostream>
#include <vector>

void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<int> v;
    for(int i = 0; i < n; ++i){
        int ai = 0;
        std::cin >> ai;
        v.push_back(ai);
    }
    int d = 0;
    std::cin >> d;
    v.erase(v.begin() + d);

    int a = 0, b = 0;
    std::cin >> a >> b;
    
    for(int i = b + 1; i < n - 1; ++i){
        v[i - (b - a + 1)] = v[i];
    }
    
    for(int i = a; i <= b; ++i){
        v.pop_back();
    }

    if (v.empty()){
        std::cout << "Empty";
    }
    else{
        for(auto vi : v){
            std::cout << vi << " ";
        }
    }
    std::cout << std::endl;
    return;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}