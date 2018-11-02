// Greedy strategy: finish as early as possible
#include <iostream>
#include <vector>
#include <algorithm>

void testcase(){
    int n;
    std::cin >> n;
    std::vector<std::pair<int, int> > boats(n);   // store <ring_pos, length> pair
    for(int i = 0; i < n; ++i){
        std::cin >> boats[i].second >> boats[i].first;
    }
    // sort the boats by the ring position 
    std::sort(boats.begin(), boats.end());
    int cur_end = -INT32_MAX;    // 当前结束位置
    int last_end = -INT32_MAX;   // 前一个船的结束位置
    int maxNum = 0;
    for(int i = 0; i < n; ++i){
        int earlyStart = boats[i].first - boats[i].second;  // the earliest possible starting position
        int lateStart = boats[i].first;    // the latest possible starting position
        int length = boats[i].second;

        if(cur_end <= lateStart){  // add a new boat 
            last_end = cur_end;
            cur_end = std::max(cur_end, earlyStart) + length;
            maxNum++;
        }

        else{   // Replace previous boat by the new one if it ends up with a earlier finishing position 
            int temp_end = std::max(last_end, earlyStart) + length;
            if(temp_end < cur_end){
                cur_end = temp_end;
            }
        }
    }
    std::cout << maxNum << std::endl;
    
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