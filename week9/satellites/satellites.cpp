/**
 * Minimum vertex cover problems
 */
// Includes
// ========
// STL includes
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
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

void print(std::set<Vertex> pts){
    for(Vertex v : pts){
        std::cout << v << " ";
    }
}

void testcase(){
    int g, s, l; // g: # of ground stations, s: # of satellites, l: # of links
    std::cin >> g >> s >> l;

    Graph G(g + s);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    for(int i = 0; i < l; ++i){
        int gi, si;
        std::cin >> gi >> si;
        eaG.addEdge(gi, si + g, 1);
    }

    Vertex src = boost::add_vertex(G);
    Vertex sink = boost::add_vertex(G);
    for(int i = 0; i < g; ++i){
        eaG.addEdge(src, i, 1);
    }

    for(int i = 0; i < s; ++i){
        eaG.addEdge(g + i, sink, 1);
    }
    
    // Compute bipartite maximal matching
    int flow = boost::push_relabel_max_flow(G, src, sink);

    // store the vertices for minimal vertex covering
    std::set<Vertex> ground_points;
    std::set<Vertex> satellite_points;
    
    // BFS on residual capacity map
    std::vector<bool> visited(boost::num_vertices(G), false);
    std::queue<Vertex> Q;
    visited[src] = true;
    Q.push(src);
    while(!Q.empty()){
        const int u = Q.front();
        Q.pop();
        OutEdgeIt ebeg, eend;
        for(boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg){
            const int v = boost::target(*ebeg, G);
            if(rescapacitymap[*ebeg] == 0 || visited[v]) continue;
            visited[v] = true;
            Q.push(v);
        }
    }

    for(int i = 0; i < g + s; ++i){
        // Unviisted in Left side
        if(i < g){
            if(visited[i] == false)
                ground_points.insert(i);
        }
        // Visited in Right side
        else{
            if(visited[i] == true)
                satellite_points.insert(i - g);
        }
    }

    std::cout << ground_points.size() << " " << satellite_points.size() << "\n";
    if(ground_points.size() == 0 && satellite_points.size() == 0)
        return;
    else{
        print(ground_points);
        print(satellite_points);
        std::cout << "\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
}