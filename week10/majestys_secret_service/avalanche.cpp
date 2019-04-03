/*
 * Algorithm: Dijkstra + bipartite matching (minimum bottleneck matching)
 * binary search the result
 */ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					UGraph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// int BinarySearch(int a, int s, std::vector<std::vector<int> >& dist){

// }

void testcase(){
    /*
    * n: number of vertices
    * m: number of edges
    * a: number of agents
    * s: number of shelters
    * c: capacity of eahc sheter
    * d: time at shelter
    */
    int n, m, a, s, c, d;
    std::cin >> n >> m >> a >> s >> c >> d;
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    for(int i = 0; i < m; ++ i){
        char w;
        int x, y, z;
        std::cin >> w >> x >> y >> z;
        if(w == 'S'){
            Edge e; bool success;
            boost::tie(e, success) = boost::add_edge(x, y, G);
            weightmap[e] = z;
        }
        else if(w == 'L'){
            Edge e, re; bool success;
            boost::tie(e, success) = boost::add_edge(x, y, G);
            boost::tie(re, success) = boost::add_edge(y, x, G);
            weightmap[e] = z; 
            weightmap[re] = z;
        }
    }

    // Compute pairwise shortest path from i-th agent to j-th shelter
    std::vector< std::vector<int> > distmap(a, std::vector<int>(n));
    std::vector<std::vector<int> > T(a, std::vector<int>(s, INT_MAX));

    for(int i = 0; i < a; ++i){
        int start;
        std::cin >> start;
        boost::dijkstra_shortest_paths(G, start,
            distance_map(boost::make_iterator_property_map(distmap[i].begin(), boost::get(boost::vertex_index, G))));
    }
    
    int max_dist = 0;
    for(int j = 0; j < s; ++j){
        int tar;
        std::cin >> tar;
        for(int i = 0; i < a; ++i){
            T[i][j] = distmap[i][tar];
            if(T[i][j] < INT_MAX)  // Attention here !
                max_dist = std::max(max_dist, T[i][j]);
        }
    }

    // Find the minimal of max edge weight in matching by binary search
    int low = 0, high = max_dist + c * d;
    int min_time = max_dist + c * d;
    while(low <= high){
        int t = low + (high - low) / 2;
        UGraph bG(s + a);
        for(int i = 0; i < a; ++i){
            for(int j = 0; j < s; ++j){
                for(int k = 0; k < c; ++k){
                    if(T[i][j] == INT_MAX)
                        continue;
                    if(T[i][j] + (k + 1) * d <= t){
                        boost::add_edge(i, a + k * s + j, bG); 
                    }
                }
            }
        }
        std::vector<int> matemap(boost::num_vertices(bG));
        boost::edmonds_maximum_cardinality_matching(bG, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, bG)));
        int matchingsize = matching_size(bG, boost::make_iterator_property_map(matemap.begin(), get(boost::vertex_index, bG)));

        if(matchingsize == a){
            min_time = t;
            high = t - 1;
        }
        else{
            low = t + 1;
        }
    }

    std::cout << min_time << "\n";
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