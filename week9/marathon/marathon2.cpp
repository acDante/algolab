// STL includes
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					uGraph;
typedef boost::graph_traits<uGraph>::edge_descriptor		u_Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<uGraph>::vertex_descriptor		u_Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<uGraph>::edge_iterator		u_EdgeIt;		// to iterate over all edges
typedef boost::property_map<uGraph, boost::edge_weight_t>::type	u_WeightMap;	// property map to access the interior property edge_weight_t

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
    int n, m, s, f;
    std::cin >> n >> m >> s >> f;
    std::map<u_Edge, int> cmap;
    uGraph uG(n);
    u_WeightMap wmap = boost::get(boost::edge_weight, uG);
    for(int i = 0; i < m; ++i){
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        u_Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(a, b, uG);
        wmap[e] = d;
        cmap[e] = c;
    }

    // Compute the shortest path from the source
    std::vector<int> distFromS(boost::num_vertices(uG));
    std::vector<int> distFromT(boost::num_vertices(uG));
    std::vector<int> predmap(boost::num_vertices(uG));
	boost::dijkstra_shortest_paths(uG, s, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, uG))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distFromS.begin(), boost::get(boost::vertex_index, uG))));	// distance map as Named Parameter
    
    boost::dijkstra_shortest_paths(uG, f, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, uG))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distFromT.begin(), boost::get(boost::vertex_index, uG))));	// distance map as Named Parameter
    int dist = distFromS[f];
    if(dist == 0 || dist == INT_MAX){
        std::cout << "0\n";
        return;
    }
    // Construct graph with only edges on shortest path
    Graph G(n);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    u_EdgeIt ebeg, eend;
    for(boost::tie(ebeg, eend) = boost::edges(uG); ebeg != eend; ++ebeg){
        int u = source(*ebeg, uG);
        int v = target(*ebeg, uG);
        // Attention : do not add bi-directional edges here 
        if(distFromS[u] + wmap[*ebeg] + distFromT[v] == dist){
            eaG.addEdge(u, v, cmap[*ebeg]);
        }
        else if(distFromS[v] + wmap[*ebeg] + distFromT[u] == dist){
            eaG.addEdge(v, u, cmap[*ebeg]);
        }
    }
    // Compute max flow on final graph
    long flow = boost::push_relabel_max_flow(G, s, f);
    std::cout << flow << "\n";
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
