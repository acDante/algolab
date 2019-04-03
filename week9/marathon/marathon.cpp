// MCMF + Binary Search
// 1. set cost to edge length => shortest paths are preferred when minimizing the cost
//    check if all the paths are shortest path = check whether the mincost is a multiple of shortest path length
// 2. Binary search the maximal possible flow

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					uGraph;
typedef boost::graph_traits<uGraph>::edge_descriptor		uEdge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::property_map<uGraph, boost::edge_weight_t>::type	uWeightMap;	// property map to access the interior property edge_weight_t


// BGL Graph definitions
// ===================== 
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, Traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > Graph; // new!
// Interior Property Maps
typedef boost::property_map<Graph, boost::edge_capacity_t>::type      EdgeCapacityMap;
typedef boost::property_map<Graph, boost::edge_weight_t >::type       EdgeWeightMap; // new!
typedef boost::property_map<Graph, boost::edge_residual_capacity_t>::type ResidualCapacityMap;
typedef boost::property_map<Graph, boost::edge_reverse_t>::type       ReverseEdgeMap;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::graph_traits<Graph>::out_edge_iterator  OutEdgeIt; // Iterator

class EdgeAdder {
    Graph &G;
    EdgeCapacityMap &capacitymap;
    EdgeWeightMap &weightmap;
    ReverseEdgeMap  &revedgemap;

public:
    EdgeAdder(Graph &G, EdgeCapacityMap &capacitymap, EdgeWeightMap &weightmap, ReverseEdgeMap &revedgemap) 
        : G(G), capacitymap(capacitymap), weightmap(weightmap), revedgemap(revedgemap) {}

    void addEdge(int u, int v, long c, long w) {
        Edge e, rev_e;
        boost::tie(e, boost::tuples::ignore) = boost::add_edge(u, v, G);
        boost::tie(rev_e, boost::tuples::ignore) = boost::add_edge(v, u, G);
        capacitymap[e] = c;
        weightmap[e] = w; // new!
        capacitymap[rev_e] = 0;
        weightmap[rev_e] = -w; // new
        revedgemap[e] = rev_e; 
        revedgemap[rev_e] = e; 
    }
};

void testcase(){
    int n, m, s, f;
    std::cin >> n >> m >> s >> f;
    
    uGraph uG(n);
    uWeightMap wmap = boost::get(boost::edge_weight, uG);

    Graph G(n);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);
    
    for(int i = 0; i < m; ++i){
        int u, v, cap, w;
        std::cin >> u >> v >> cap >> w;
        eaG.addEdge(u, v, cap, w);
        eaG.addEdge(v, u, cap, w);
        
        uEdge e; bool success;
        boost::tie(e, success) = boost::add_edge(u, v, uG);
        wmap[e] = w;
    }
    // Add super sink after f to constain the flow
    int sink = boost::add_vertex(G);
    eaG.addEdge(f, sink, INT_MAX, 0);

    // Compute the shortest path length from s to f
    std::vector<int> distmap(n);
    boost::dijkstra_shortest_paths(uG, s, 
        distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));
    long ssplen = distmap[f];

    boost::successive_shortest_path_nonnegative_weights(G, s, sink);
    long MAX_FLOW = 0;
    OutEdgeIt e, eend;
    for (boost::tie(e, eend) = boost::out_edges(boost::vertex(s, G), G); e != eend; ++e) {
        MAX_FLOW += capacitymap[*e] - rescapacitymap[*e];
    }

    // Binary Search the maximal possible flow such that 
    // totalLength (min cost) = flow * shortest path length
    long low = 0, high = MAX_FLOW;
    long maxflow = 0;
    while(low <= high){
        long flow = low + (high - low) / 2;
        Edge e; bool success;
        boost::tie(e, success) = boost::edge(f, sink, G);
        capacitymap[e] = flow;

        boost::successive_shortest_path_nonnegative_weights(G, s, sink);
        long totalLength = boost::find_flow_cost(G);
        if(totalLength == flow * ssplen){
            maxflow = flow;
            low = flow + 1;
        }
        else if(totalLength > flow * ssplen){
            high = flow - 1;
        }
    }
    std::cout << maxflow << "\n";

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
