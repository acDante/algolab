// STL includes
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges

void testcase(){
    int n, c, f;
    std::cin >> n >> c >> f;
    std::vector<std::vector<std::string> > features(n, std::vector<std::string>());
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < c; ++j){
            std::string ci;
            std::cin >> ci;
            features[i].push_back(ci);
        }
        std::sort(features[i].begin(), features[i].end());
    }

    Graph G(n);
    for(int i = 0; i < n; ++i){
        for(int j = i; j < n; ++j){
            std::vector<std::string> intersection;
            std::set_intersection(features[i].begin(), features[i].end(),
                        features[j].begin(), features[j].end(),
                        std::back_inserter(intersection));
            if(intersection.size() > f){
                Edge e; bool success;
                boost::tie(e, success) = boost::add_edge(i, j, G);
            }
        }
    }

    // Edmonds' maximum cardinality matching
	// =====================================
	std::vector<Vertex> matemap(n);		// We MUST use this vector as an Exterior Property Map: Vertex -> Mate in the matching
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
    int matchingsize = matching_size(G, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, G)));
    if(matchingsize == n / 2){
        std::cout << "not optimal" << std::endl;
    }
    else{
        std::cout << "optimal" << std::endl;
    }
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
