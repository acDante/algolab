// Graph model + Dijkstra 
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t


int NUM_START = 3;    // number of starting points

// Check the feasibility of indices (i, j)
bool feasible(int i, int j, int k){
    return i >= 1 && i <= k && j >= 1 && j <= i;
}

void testcase(){
    int k;   // number of layers
    std::cin >> k;
    std::vector<std::vector<int> > val(k + 1, std::vector<int>(k + 1, 0));
    std::vector<std::vector<int> > index(k + 1, std::vector<int>(k + 1, 0));  // map position (i, j) to graph index
    
    Graph G((k + 1) * k / 2);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    for(int i = 1; i <= k; ++i){
        for(int j = 1; j <= i; ++j){
            std::cin >> val[i][j];
            index[i][j] = i * (i - 1) / 2 + j - 1;
            Edge e, re; bool success;
            if(feasible(i, j - 1, k)){
                boost::tie(e, success) = boost::add_edge(index[i][j], index[i][j - 1], G);
                weightmap[e] = val[i][j - 1];
                boost::tie(re, success) = boost::add_edge(index[i][j - 1], index[i][j], G);
                weightmap[re] = val[i][j];
            }
            if(feasible(i - 1, j - 1, k)){
                boost::tie(e, success) = boost::add_edge(index[i][j], index[i - 1][j - 1], G);
                weightmap[e] = val[i - 1][j - 1];
                boost::tie(re, success) = boost::add_edge(index[i - 1][j - 1], index[i][j], G);
                weightmap[re] = val[i][j];
            }
            if(feasible(i - 1, j, k)){
                boost::tie(e, success) = boost::add_edge(index[i][j], index[i - 1][j], G);
                weightmap[e] = val[i - 1][j];
                boost::tie(re, success) = boost::add_edge(index[i - 1][j], index[i][j], G);
                weightmap[re] = val[i][j];
            }
        }
    }

    // Compute shortest paths from 3 starting vertices
    std::vector<int> start = {index[1][1], index[k][1], index[k][k]};
    std::vector<std::vector<long> > distmap(NUM_START, std::vector<long>(boost::num_vertices(G)));
    for(int i = 0; i < NUM_START; ++i){
        boost::dijkstra_shortest_paths(G, start[i], 
            distance_map(boost::make_iterator_property_map(distmap[i].begin(), boost::get(boost::vertex_index, G))));
    }

    // Find the minimum length among all the possbile connected paths
    long min_dist = std::numeric_limits<long>().max();
    for(int i = 1; i <= k; ++i){
        for(int j = 1; j <= i; ++j){
            long cur_dist = 0;
            for(int k = 0; k < NUM_START; ++k){
                cur_dist += distmap[k][index[i][j]];
            }
            min_dist = std::min(min_dist, cur_dist - 2 * val[i][j]);   // remember to avoid double-counting
        }
    }
    std::cout << min_dist << "\n";
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
