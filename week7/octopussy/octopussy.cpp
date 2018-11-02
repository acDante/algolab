// Implement with min heap with dynamic priority
#include <iostream>
#include <queue>

typedef std::pair<int, int> PII;
void testcase(){
    int n;
    std::cin >> n;
    std::vector<int> bombs(n);
    std::vector<bool> deactivated(n, 0);
    std::vector<PII> minHeap;
    // std::priority_queue <PII, std::vector<PII>, std::greater<int> > pq;
    for(int i = 0; i < n; ++i){
        std::cin >> bombs[i];
        minHeap.push_back(std::make_pair(bombs[i], i));
    }
    // std::push_heap(minHeap.begin(), minHeap.end());
    std::make_heap(minHeap.begin(), minHeap.end(), std::greater<PII>());
    for(PII pi : minHeap){
        std::cout << pi.first << " " << pi.second << std::endl;
    }
    std::cout << "***" << std::endl;
    int t = 0;
    bool fail = false;   // Indicate whether we can find a valid permutation 
    while(minHeap.size()!=0){
        PII cur_bomb = minHeap.front();    // Find the bomb with minimal time among current activate bombs
        std::cout << cur_bomb.first << " " << cur_bomb.second << std::endl;
        int bomb_index = cur_bomb.second;
        int bomb_time = cur_bomb.first;
        bool deactivatable = false;
        int left_child = 2 * bomb_index + 1;
        int right_child =  2 * bomb_index + 2;

        if(bomb_time <= t){
            fail = true;
            break;
        }

        if(bomb_index >= (n - 1) / 2){
            deactivatable = true;
        }
        else{
            if(deactivated[left_child] && deactivated[right_child])
                deactivatable = true;
        }

        if(deactivatable){
            std::pop_heap(minHeap.begin(), minHeap.end(), std::greater<PII>());
            minHeap.pop_back();
            deactivated[bomb_index] = 1;
            t++;
        }
        else{
            std::cout << "arrive here";
            if(!deactivated[left_child] && !deactivated[right_child]){
                minHeap[left_child].first = std::min(bomb_time - 1, bombs[left_child]);
                minHeap[right_child].first = std::min(bomb_time - 2, bombs[right_child]);
            }
            
            else if(!deactivated[left_child]){
                minHeap[left_child].first = std::min(bomb_time - 1, bombs[left_child]);
            }

            else if(!deactivated[right_child]){
                minHeap[right_child].first = std::min(bomb_time - 1, bombs[right_child]);
            }
            std::cout << minHeap[left_child].first << " || " << minHeap[left_child].second << "\n";
            std::cout << minHeap[right_child].first << " || " << minHeap[right_child].second << "\n";

        }
        // std::pop_heap(minHeap.begin(), minHeap.end(), std::greater<PII>());
        // minHeap.pop_back();
        std::make_heap(minHeap.begin(), minHeap.end(), std::greater<PII>());
        if(bomb_time == 5){
            // for(PII pi : minHeap){
            //     std::cout << pi.first << " " << pi.second << std::endl;
            // }
            // std::cout << "***" << std::endl;
            break;
        }
    }

    if(fail){
        std::cout << "no" << "\n";
    }
    else{
        std::cout << "yes" << "\n";
    }
}

int main(){
    int t; std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}