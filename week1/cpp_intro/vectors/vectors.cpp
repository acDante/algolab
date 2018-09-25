// Another method: use another vector to store remaining elements
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

    std::vector<int> v2;
    for(int i = 0; i < n - 1; ++i){
        if(i >= a && i <= b)
            continue;
        v2.push_back(v[i]);
    }

    if (v2.empty()){
        std::cout << "Empty";
    }
    else{
        for(auto vi : v2){
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
