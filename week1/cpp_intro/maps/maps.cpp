#include <iostream>
#include <map>
#include <set>
#include <algorithm>

typedef std::pair<std::string, int> PA;
bool my_compare(PA const & a, PA const & b){
    return a.first != b.first?  a.first < b.first : a.second < b.second;
}

void testcase(){
    int q = 0;
    std::cin >> q;
    std::multimap<std::string, int> m;
    for(int i = 0; i < q; ++i){
        int a = 0;
        std::string b;
        std::cin >> a >> b;
        if(a == 0){
            auto ret = m.equal_range(b);
            if (m.find(b)!= m.end()){
                for(auto itr = ret.first; itr != ret.second;){
                    itr = m.erase(itr);
                }
            }
        }
        if(a > 0){
            m.insert(std::make_pair(b, a));
        }
    }
    // std::sort(m.begin(), m.end(), my_compare);
    std::string s;
    std::cin >> s;
    auto ret = m.equal_range(s);
    if (m.find(s) == m.end()){
        std::cout << "Empty";
    }
    else{
        std::multiset<int> ms;    // store and sort the values 
        for(auto itr = ret.first; itr != ret.second; itr++){
            ms.insert(itr->second);
        }
        for(int i : ms){
            std::cout << i << " ";
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
