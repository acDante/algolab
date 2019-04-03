/**
 * Algorithm: split and list
 * Method: 1. split the switches into two groups, compute all the possible light pattern vector and min number of switches in each group, save to map 
 *         2. Sort the map key in increasing order (a feature provided by std::map)
 *         3. For each light pattern in map1, find the complementary pattern in map2
 */
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

typedef std::map<std::vector<int>, int> MinClickMap;
class Room{
    public:
        int on;
        int off;
    
    Room(){
        on = 0; off = 0;
    }
    Room(int on, int off){
        this->on = on;
        this->off = off;
    }
};

int N, M;
std::vector<std::vector<Room> > lights;

void computeClicks(int start, int end, int numRoom, MinClickMap& clickmap){
    for(int s = 0; s < 1 << (end - start); ++s){
        std::vector<int> bright(M, 0);
        int count = 0;
        for(int i = 0; i < end - start; ++i){
            if(s & 1 << i){
                count++;
                for(int j = 0; j < numRoom; ++j){
                    bright[j] += lights[start + i][j].off;
                }
            }
            else{
                for(int j = 0; j < numRoom; ++j){
                    bright[j] += lights[start + i][j].on;
                }
            }
        }
        if(clickmap.find(bright) != clickmap.end()){
            clickmap[bright] = std::min(clickmap[bright], count);
        }
        else{
            clickmap[bright] = count;
        }
    }
}

void testcase(){
    // int N, M;  // N: # of switches, M: # of rooms
    std::cin >> N >> M;
    std::vector<int> b(M, 0); // brightness
    for(int i = 0; i < M; ++i){
        std::cin >> b[i];
    }

    lights = std::vector<std::vector<Room> >(N, std::vector<Room>(M));
    // std::vector<std::vector<Room> > lights(N, std::vector<Room>(M));
    for(int i = 0; i < N; ++i){
        for(int j = 0;  j < M; ++j){
            std::cin >> lights[i][j].on >> lights[i][j].off;
        }
    }

    MinClickMap ClickMap_left, ClickMap_right;
    computeClicks(0, N/2, M, ClickMap_left);
    computeClicks(N/2, N, M, ClickMap_right);

    // std::sort(L2.begin(), L2.end(), my_comparator);
    //print(L2);
    
    int minCount = std::numeric_limits<int>().max();
    bool found = false;
    for(auto val : ClickMap_left){
        std::vector<int> target(M, 0);
        for(int i = 0; i < M; ++i){
            target[i] = b[i] - val.first[i];
        }
        auto itr = ClickMap_right.find(target);
        if(itr != ClickMap_right.end()){
            minCount = std::min(minCount, val.second + itr->second);
            found = true;
        }
    }

    if(found)
        std::cout << minCount << "\n";
    else
        std::cout << "impossible\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
