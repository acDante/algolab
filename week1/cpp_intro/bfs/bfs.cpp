// 利用adjacent list来描述图中的节点关系 + Dijkstra's algorithm
// 感觉还不是很熟练....需要再多练几题
#include <iostream>
#include <vector>
#include <queue>
typedef std::vector<int> VI;

void testcase(){
    int n = 0, m = 0, v = 0;
    std::cin >> n >> m >> v;
    std::vector<VI> neighbors(n);
    // construct the neighbor list 
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0;
        std::cin >> a >> b;
        neighbors[a].push_back(b);
        neighbors[b].push_back(a);
    }
    std::vector<int> dist(n, 1001);    // initialize the distance to be infinitely large (here can be set to 1001 since n <= 1000)
    std::vector<bool> visited(n, false);
    std::queue<int> q;    
    q.push(v);
    dist[v] = 0;

    while(!q.empty()){
        int current = q.front();
        visited[current] = true;
        q.pop();

        if(!neighbors[current].empty()){
            for(int ni : neighbors[current]){
                if(!visited[ni] && dist[ni] > dist[current] + 1){
                    q.push(ni);
                    dist[ni] = dist[current] + 1;
                }
            }
        }
    }

    for (int di : dist){
        if(di == 1001)
            std::cout << -1 << " ";
        else
            std::cout << di << " ";
    }
    
    std::cout << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0;i < t; ++i){
        testcase();
    }
    return 0;
}
