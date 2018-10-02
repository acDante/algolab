// sliding window 
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <cmath>

typedef std::pair<int, int> Interval;

void print_intervals(std::vector<Interval> v){
    for(Interval vi : v){
        std::cout << "(" << vi.first << ", " << vi.second << ") ";
    }
    std::cout << std::endl;
}

void print(std::vector<int> v){
    std::cout << v[0];
    for(int i = 1; i < v.size(); ++i){
        std::cout << " " << v[i];
    }
    std::cout << std::endl;
}

void testcase(){
    int n = 0;
    std::cin >> n;
    std::vector<int> parasols;
    parasols.reserve(n);
    for(int i = 0; i < n; ++i){
        int xi = 0; 
        std::cin >> xi;
        parasols.push_back(xi);
    }
    // sort parasols in increasing order
    std::sort(parasols.begin(), parasols.end());
    int i = 0, j = 0;
    int cur_num = 1, max_num = 1;
    std::vector<Interval> candidates;

    // use sliding window to find invertals that cover max number of parasols
    while(j < parasols.size()){
        if(cur_num > max_num){
            candidates.clear();
            candidates.push_back(std::make_pair(parasols[i], parasols[j]));
            max_num = cur_num;
        }
        else if(cur_num == max_num){
            candidates.push_back(std::make_pair(parasols[i], parasols[j]));
        }

        j++;
        if(parasols[j] - parasols[i] <= 200){
            cur_num++;
        }
        else{
            i++;
        }
    }
    // print_intervals(candidates);

    int min_dist = INT32_MAX;
    std::vector<int> bar_pos;
    for(Interval inter: candidates){
        // the position with minimal longest distance  = find the midpoint
        int i = inter.first, j = inter.second;
        double midpoint = (i + j) / 2.0;
        int left_pos = floor(midpoint), right_pos = ceil(midpoint);
        int dist = 0;
        if(left_pos == right_pos)
            dist = left_pos - i;
        else
            dist = left_pos - i + 1;
        
        bool FindNew = false;    // to indicate whether we find a new possible beach bar point
        if(dist < min_dist){
            bar_pos.clear();
            min_dist = dist;
            FindNew = true;
        }
        else if(dist == min_dist){
            FindNew = true;
        }
        if(FindNew){
            if(left_pos == right_pos)
                bar_pos.push_back(left_pos);
            else{
                bar_pos.push_back(left_pos);
                bar_pos.push_back(right_pos);
            }
        }
    }
    std::cout << max_num << " " << min_dist << std::endl;
    print(bar_pos);

}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
