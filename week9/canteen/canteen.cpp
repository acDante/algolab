#include <iostream>
#include <cstdlib>
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

void testcase(){
    int n; std::cin >> n;
    Graph G(n);
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    int src = boost::add_vertex(G);
    int tar = boost::add_vertex(G);
    int MAXP = 20;    // Maximum profit per menu
    for(int i = 0; i < n; ++i){
        int ai, ci;
        std::cin >> ai >> ci;
        eaG.addEdge(src, i, ai, ci);
    }

    int num_std = 0;   // total number of students
    for(int i = 0; i < n; ++i){
        int si, pi;
        std::cin >> si >> pi;
        eaG.addEdge(i, tar, si, MAXP - pi);
        num_std += si;
    }
    for(int i = 0; i < n - 1; ++i){
        int vi, ei;
        std::cin >> vi >> ei;
        eaG.addEdge(i, i + 1, vi, ei);
    }
    boost::successive_shortest_path_nonnegative_weights(G, src, tar);
    long minCost = boost::find_flow_cost(G);
    long maxFlow = 0;
    OutEdgeIt e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src, G), G); e != eend; ++e){
        maxFlow += capacitymap[*e] - rescapacitymap[*e];
    }
    long maxProfit = maxFlow * MAXP - minCost;
    if(maxFlow < num_std)
        std::cout << "impossible ";
    else
        std::cout << "possible ";
    std::cout << maxFlow << " " << maxProfit << "\n";
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
