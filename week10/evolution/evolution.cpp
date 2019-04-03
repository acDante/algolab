// linear search solution: worst case complexity O(n)
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

typedef std::pair<std::string, int> Species;
void print(std::vector<std::string> names){
    for(int i = 0; i < names.size() - 1; ++i){
        std::cout << names[i] << " ";
    }
    std::cout << names[names.size() - 1] << "\n";
}
void testcase(){
    int n, q;
    std::cin >> n >> q;
    std::map<std::string, int> ageMap;
    std::map<std::string, std::string> parentMap;

    for(int i = 0; i < n; ++i){
        std::string s; int a;
        std::cin >> s >> a;
        ageMap[s] = a;
        parentMap[s] = s;
    }

    for(int i = 0; i < n - 1; ++i){
        std::string s, p;
        std::cin >> s >> p;
        parentMap[s] = p;
    }

    std::vector<std::string> names;
    for(int i = 0; i < q; ++i){
        std::string s; int b;
        std::cin >> s >> b;
        
        int age = ageMap[s];
        std::string p = s;
        std::string result = p;
        while(age <= b){
            if(parentMap[p] == p){
                result = p;
                break;
            }
            result = p;
            p = parentMap[p];
            age = ageMap[p];
            // std::cout << p << " " << age << "\n";
        }
        names.push_back(result);
    }
    print(names);
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
