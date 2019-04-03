/**
 * Algorithm: split the students into 2 lists, list all the possible 3^n assignemnts of each subset, (team[2] are the non-selected students)
 * Store # of each pair < sum_difference, non-selected size > into countMap
 * Store each <size, count> pair of countMap1 into finalMap, using "sum_difference" as the key
 * For each "sum_difference" of CountMap2, count all the feasible solutions in countMap1  
 */  
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

typedef std::pair<long, long>  PLL;

std::vector<int> skill;

// @ compute difference of the sum: S1 - S2
long sum_difference(std::vector<int>& v, std::vector<int>& w){
    int s1 = std::accumulate(v.begin(), v.end(), 0);
    int s2 = std::accumulate(w.begin(), w.end(), 0);
    long diff = long(s1 - s2);
    return diff;
}

// @ List all the possible assignments
void list_assignment(std::map<PLL, long>& countMap, int left, int right, std::vector<std::vector<int> >& teams){
    if(left == right){
        long diff = sum_difference(teams[0], teams[1]);
        long non_selected = teams[2].size();
        auto key = std::make_pair(diff, non_selected);
        if(countMap.find(key) != countMap.end()){
            countMap[key]++;
        }
        else
            countMap[key] = 1;
        return;
    }
    for(int i = 0; i < teams.size(); ++i){
        teams[i].push_back(skill[left]);
        list_assignment(countMap, left + 1, right, teams);
        teams[i].pop_back();
    }
}

void testcase(){
    int n, k;
    std::cin >> n >> k;
    skill.clear(); skill.resize(n);
    for(int i = 0; i < n; ++i){
        std::cin >> skill[i];
    }

    std::map<PLL, long> countMap_left, countMap_right;
    std::vector<std::vector<int> > teams_left(3, std::vector<int>()), teams_right(3, std::vector<int>());
    
    list_assignment(countMap_left, 0, n / 2, teams_left);
    list_assignment(countMap_right, n / 2, n, teams_right);

    std::map<long, std::vector<PLL> > finalMap;  // map[sum_difference] = std::vector< <size, count> > 
    for(auto element : countMap_left){
        finalMap[element.first.first].push_back(std::make_pair(element.first.second, element.second));
    }

    long count = 0;
    for(auto it = countMap_right.begin(); it != countMap_right.end(); it++){
        long diff = -it->first.first;   // key: difference of sum
        long size = it->first.second;   // number of non-selected students
        long cur_count = it ->second;   // count of <diff, size> pair 
        if(finalMap.find(diff) != finalMap.end()){
            auto vec = finalMap[diff];
            for(auto entry : vec){
                if(entry.first <= k - size)
                    count += cur_count * entry.second;  // Attention
            }
        }
    }
    std::cout << count << "\n";
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
