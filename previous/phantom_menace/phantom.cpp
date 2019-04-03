// Minimum Vertex Cut + Vertex Split 最小点割集问题 （和最小割很像，只是变成了割点）
// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <climits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

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
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;


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
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

void testcase(){
    int V, E, num_s, num_d;
    // V: # of vertices, E: # of edges, 
    // num_s: # of start points, num_d: # of end points
    std::cin >> V >> E >> num_s >> num_d;
    Graph G(2 * V);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    std::map<Vertex, Vertex> outVertex;
    for(int i = 0; i < V; ++i){
        outVertex[i] = i + V;
        eaG.addEdge(i, outVertex[i], 1);
    }

    for(int i = 0; i < E; ++i){
       Vertex u, v;
       std::cin >> u >> v;
       eaG.addEdge(outVertex[u], v, INT_MAX);
    }

    Vertex src = boost::add_vertex(G);
    Vertex sink = boost::add_vertex(G);
    for(int i = 0; i < num_s; ++i){
        Vertex si;
        std::cin >> si;
        eaG.addEdge(src, si, INT_MAX);
    }

    for(int i = 0; i < num_d; ++i){
        Vertex ti;
        std::cin >> ti;
        eaG.addEdge(outVertex[ti], sink, INT_MAX);
    }

    long flow = boost::push_relabel_max_flow(G, src, sink);
    std::cout << flow << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int T = 0;
    std::cin >> T;
    while(T--){
        testcase();
    }
    return 0;
}
