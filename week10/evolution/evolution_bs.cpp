#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

typedef std::pair<int, std::string> PIS;
typedef std::pair<int, int> PII;
typedef std::vector<int> VI;
typedef std::vector<std::vector<int> > VVI;

int binSearch(VI& path, VI& age, int maxAge);

void dfs(int v, VI& path, VVI& childList, VI& age, std::vector<std::vector<PII> >& queries,
         VI& results ){
    if(!queries[v].empty()){
        for(auto query : queries[v]){
            results[query.second] = binSearch(path, age, query.first);
        }
    }
    for(int w : childList[v]){
        path.push_back(w);
        dfs(w, path, childList, age, queries, results);
    }
    path.pop_back();
}

int binSearch(VI& path, VI& age, int maxAge){
    int low = 0, high = path.size() - 1;
    int res = 0;
    while(low <= high){
        int mid = (low + high) / 2;
        if(age[path[mid]] > maxAge)
            low = mid + 1;
        else{
            res = mid;
            high = mid - 1;
        }
    }
    return path[res];
}

void testcase(){
    int n, q;    // n: # of species, q: # of queries
    std::cin >> n >> q;
    std::vector<int> age(n);
    std::vector<std::string> name(n);
    std::map<std::string, int> indexMap;
    int root = 0, maxAge = 0;
    for(int i = 0; i < n; ++i){
        std::string s; int a;
        std::cin >> s >> a;
        indexMap[s] = i;
        age[i] = a;
        name[i] = s;

        if(a > maxAge){
            maxAge = a;
            root = i;
        }
    }

    std::vector<std::vector<int> > childList(n, std::vector<int>());
    for(int i = 0; i < n - 1; ++i){
        std::string s, p;
        std::cin >> s >> p;
        childList[indexMap[p]].push_back(indexMap[s]);
    }

    std::vector<std::vector<PII> > queries(n, std::vector<PII>());
    for(int i = 0; i < q; ++i){
        std::string s; int b;
        std::cin >> s >> b;
        queries[indexMap[s]].push_back(std::make_pair(b, i));
    }
    std::vector<int> results(q);
    std::vector<int> path;  path.push_back(root);

    dfs(root, path, childList, age, queries, results);
    for(int i = 0; i < q; ++i){
        std::cout << name[results[i]];
        if(i < q - 1) std::cout << " ";
    }
    std::cout << "\n";
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
