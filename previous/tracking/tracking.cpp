// Create k-level grpah + Dijkstra shortest path
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void addEdge(Graph& G, int n, int from_level, int from, int to_level, int to, int cost){
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    Edge e; bool success;
    boost::tie(e, success) = boost::add_edge(from_level * n + from, to_level * n + to, G);
    weightmap[e] = cost;
}
void testcase(){
    int n, m, k, x, y;    // n: # of vertices, m: # of edges
    std::cin >> n >> m >> k >> x >> y;
    // copy the whole graph k times, connect k-th layer to k + 1-th layer after going along river edge
    Graph G((k + 1) * n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    for(int i = 0; i < m; ++i){
        int a, b, c, d;
        std::cin >> a >> b >> c >> d;
        // Add edges at each layer
        for(int l = 0; l <= k; ++l){
            addEdge(G, n, l, a, l, b, c);
        }
        if(d == 1){
            // For river edge, add bridges between layers
            for(int l = 0; l < k; l++){
                addEdge(G, n, l, a, l + 1, b, c);
                addEdge(G, n, l, b, l + 1, a, c);
            }
        }
    }
    // Dijkstra shortest paths
	// =======================
	std::vector<Vertex> predmap(boost::num_vertices(G));	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(boost::num_vertices(G));		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	boost::dijkstra_shortest_paths(G, x, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    std::cout << distmap[k * n + y] << "\n";
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
