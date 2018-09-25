#include <iostream>
#include <set>
void testcase(){
    int q = 0;
    std::cin >> q;
    std::set<int> s;
    for(int i = 0; i < q; ++i){
        int a = 0, b = 0;
        std::cin >> a >> b;
        if(a == 0){
            s.insert(b);
        }
        if(a == 1){
            auto itr = s.find(b);
            if(itr != s.end()){
                s.erase(b);
            }
        }
    }
    if (s.empty()){
        std::cout << "Empty";
    }
    else{
        for(auto si : s){
            std::cout << si << " ";
        }
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
