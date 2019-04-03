// Find the global min cut
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <set>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::out_edge_iterator			OutEdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0;
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void print(std::vector<int> result){
    std::cout << result.size() << " ";
    for(int i = 0; i < result.size(); ++i){
        std::cout << result[i];
        if(i != result.size() - 1)
            std::cout << " ";
    }
    std::cout << "\n";
}

void testcase(){
    int n, m;
    std::cin >> n >> m;
    Graph G(n);	
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    for(int i = 0; i < m; ++i){
        int a, b, c;
        std::cin >> a >> b >> c;
        eaG.addEdge(a, b, c);
    }
    std::pair<int, int> st;
    long minCut = INT_MAX;
    for(int i = 0; i < n; ++i){
        int src = i;
        int tar = (i + 1) % n;
        long cur = boost::push_relabel_max_flow(G, src, tar);
        if(cur < minCut){
            minCut = cur;
            st = std::make_pair(src, tar);     
        }
    }
    minCut = boost::push_relabel_max_flow(G, st.first, st.second);
    std::vector<int> res;
    std::vector<int> vis(n, false);
    std::queue<int> q;
    q.push(st.first);
    vis[st.first] = true;
    while(!q.empty()){
        const int u =  q.front();
        q.pop();
        OutEdgeIt ebeg, eend;
        for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg){
            const int v = boost::target(*ebeg, G);
            if (rescapacitymap[*ebeg] == 0 || vis[v]) continue;
            vis[v] = true;
            q.push(v);
        }
    }
    // for(int i = 0; i < n; ++i){
    //     if(vis[i])
    //         res.push_back(i);
    // }
    std::cout << minCut << "\n";
    std::cout << std::count(vis.begin(), vis.end(), true);
    for(int i = 0; i < n; ++i){
        if (vis[i])
            std::cout << " " << i;
    }
    std::cout << "\n";
    //print(res);
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
