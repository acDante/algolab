// Solution to Bonus Level with MCMF 
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <climits>
#include <cassert>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

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

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map, weight map and reverse edge map
// ===============================================================
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

bool is_inbound(int x, int y, int n){
    return (x >= 0 && x < n) && (y >= 0 && y < n);
}

void testcase(){
    int n;
    std::cin >> n;
    std::vector<std::vector<int> > bonus(n, std::vector<int>(n));
    std::map<std::pair<int, int>, int> inVertex, outVertex;
    
    Graph G(2 * n * n);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    const int MAX_COST = 100;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::cin >> bonus[i][j];
            std::pair<int, int> index = std::make_pair(i, j);
            inVertex[index] = i * n + j;
            outVertex[index] = n * n + inVertex[index];
            // Vertex split
            eaG.addEdge(inVertex[index], outVertex[index], 1,  MAX_COST - bonus[i][j]);
            eaG.addEdge(inVertex[index], outVertex[index], 1, MAX_COST);
        }
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            std::pair<int, int> cur = std::make_pair(i, j);
            std::pair<int, int> right = std::make_pair(i, j + 1);
            std::pair<int, int> down = std::make_pair(i + 1, j);
            if(is_inbound(i, j + 1, n))
                eaG.addEdge(outVertex[cur], inVertex[right], 2, 0);
            if(is_inbound(i + 1, j, n))
                eaG.addEdge(outVertex[cur], inVertex[down], 2, 0);
        }
    }

    Vertex src = boost::add_vertex(G);
    Vertex sink = outVertex[std::make_pair(n - 1, n - 1)];
    eaG.addEdge(src, 0, 2, 0);
    
    boost::successive_shortest_path_nonnegative_weights(G, src, sink);
    int cost = boost::find_flow_cost(G);
    int result =  2 * (2 * n - 1) * MAX_COST - cost;

    std::cout << result << std::endl;
}

int main(){
    int T = 0;
    std::cin >> T;
    while(T--){
        testcase();
    }
    return 0;
}