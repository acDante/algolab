#include<iostream>
#include<stack>
#include<vector>
#include<set>
#include<functional>

void print(std::vector<int> T){
    for(int ti : T){
        std::cout << ti << " ";
    }
    std::cout << std::endl;
}

void testcase(){
    int n = 0, m = 0, v = 0;
    std::cin >> n >> m >> v;
    // construct the adjacency list
    std::vector<std::set<int, std::greater<int> > > neighbors(n);
    for(int i = 0; i < m; ++i){
        int a = 0, b = 0;
        std::cin >> a >> b;
        neighbors[a].insert(b);
        neighbors[b].insert(a);
    }
    std::vector<int> in(n, -1);    
    std::vector<int> out(n, -1);    
    std::vector<bool> visited(n, false);

    std::stack<int> s;
    s.push(v);
    int time = 0;
    while(!s.empty()){
        int cur = s.top();
        s.pop();
        //std::cout << "visiting " << cur << std::endl;
        if(!visited[cur]){
            visited[cur] = true;
            in[cur] = time;
            time++;
        }
        // consider border case: 图里有环的情况!!!!!!
        if(out[cur] != -1){
            continue;
        }

        std::vector<int> children;
        for(int ni : neighbors[cur]){
            if(!visited[ni]){
                children.push_back(ni);
            }
        }

        if(children.empty()){
            out[cur] = time;
            //std::cout << "exiting " << cur << std::endl;
            time++;
        }
        else{
            s.push(cur);
            for(int ci : children){
                s.push(ci);
            }
            children.clear();
        }

        // std::cout << "current top:=" << cur << std::endl;
        // std::cout << "current time:=" << time << std::endl;        
    }

    print(in);
    print(out);
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
