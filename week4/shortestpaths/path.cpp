#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

int main(){
    int n, m, q;
    std::cin >> n >> m >> q;
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    
    std::vector<std::pair<int, int> > positions(n);  // store <lon, lat>
    for(int i = 0; i < n; ++i){
        std::cin >> positions[i].first >> positions[i].second;
    }
    // build the graph
    for(int i = 0; i < m; ++i){
        int u, v, w;
        std::cin >> u >> v >> w;
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(u, v, G);
        weightmap[e] = w;
    }
    // answer the shortest path query
    for(int i = 0; i < q; ++i){
        int s, t;
        std::cin >> s >> t;
        std::vector<Vertex> predmap(n);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	    std::vector<int> distmap(n);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source

	    boost::dijkstra_shortest_paths(G, s, // We MUST provide at least one of the two maps
		    boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		    distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
        if(distmap[t] < INT_MAX){
            std::cout << distmap[t] << std::endl;
        }
        else{
            std::cout << "unreachable" << std::endl;
        }
    }

    return 0;
}