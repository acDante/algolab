#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>
#include <climits>

void print(std::vector<int>& vec){
    for(int v : vec){
        std::cout << v << " ";
    }
    std::cout << "\n";
}

void testcase(){
    int n;    // n: # of balls
    std::cin >> n;
    std::vector<int> t(n);    // bomb time
    for(int i = 0; i < n; ++i){
        std::cin >> t[i];
    }

    // Update the latest bomb time for each node by BFS in top-down manner
    // For each node i, t[i] = min{t[parent[i]] - 1, t[i]} 
    
    std::queue<int> q;
    std::priority_queue<int, std::vector<int>, std::greater<int> > pq;  // Use std::greater to have min_heap 
    q.push(0);
    pq.push(t[0]);
    while(!q.empty()){
        int parent = q.front();
        int left = 2 * parent + 1, right = 2 * parent + 2;
        // std::cout << parent <<  " ";
        q.pop();
        if(left < n){
            t[left] = std::min(t[left], t[parent] -1);
            q.push(left);
            pq.push(t[left]);
        }
        if(right < n){
            t[right] = std::min(t[right], t[parent] - 1);
            q.push(right);
            pq.push(t[right]);
        }
    }

/*     print(t);
    std::cout << "Priority Queue: \n";
    while(!pq.empty()){
        int cur = pq.top();
        pq.pop();
        std::cout << cur << " ";
    }
    std::cout << "\n"; */

    int time = 0;
    bool success = true;
    while(!pq.empty()){
        int cur = pq.top();
        // std::cout << time << " " << cur << "\n";
        if(time < cur){
            pq.pop();
            time++;
        }
        else{
            success = false;
            break;
        }
    }
    if(success)
        std::cout << "yes\n";
    else
        std::cout << "no\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}