#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor	Vertex;
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;

void testcase(){
    int n, m;
    std::cin >> n >> m;
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    for(int i = 0; i < m; ++i){
        int u, v, w;
        std::cin >> u >> v >> w;
        Edge e; bool success;
        boost::tie(e, success) =  boost::add_edge(u, v, G);
        weightmap[e] = w;
    }

    // compute the maximal shortest path
    std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	Vertex start = 0;
	boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    int maxDist = 0;
    for(int i = 0; i < n; ++i){
        if(distmap[i] > maxDist){
            maxDist = distmap[i];
        }
    }

    // compute minimal spanning tree
    std::vector<Vertex> primpredmap(n);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)));
    int total_weight = 0;
    for(int i = 0; i < n; ++i){
        if(primpredmap[i] != i){
            Edge ei; bool success;
            boost::tie(ei, success) = boost::edge(i, primpredmap[i], G);
            total_weight += weightmap[ei];
        }
    }

    std::cout << total_weight << " " << maxDist << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
}