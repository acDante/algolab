/**
 * Notes for this problem: construct time space graph + min cost max flow
 * To make the cost nonnegative, we need to add "delta_t * maxP" for each edge,
 * so that each s->t path increases by maxT * maxP
 */ 
#include <iostream>
#include <vector>
#include <set>
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

class Request{
    public:
        int s;
        int t;
        int d;
        int a;
        int p;
    Request(): s(0), t(0), d(0), a(0), p(0) {}
    Request(int s, int t, int d, int a, int p): s(s), t(t), d(d), a(a), p(p) {}
};

void testcase(){
    int N, S;    // N: # of rqs, S: # of stations
    std::cin >> N >> S;
    std::vector<Request> rqs(N);
    std::vector<int> cars(S);
    for(int i = 0; i < S; ++i){
        std::cin >> cars[i];
    }

    int MAXP = 100;
    int MAXT = 100000;
    int MAXL = 100;
    std::vector<std::set<int> > timeline(S, std::set<int>());
    std::vector<std::vector<int> > time(S, std::vector<int>());
    for(int i = 0; i < S; ++i){
        timeline[i].insert(0);
        timeline[i].insert(MAXT);
    }
    for(int i = 0; i < N; ++i){
        int si, ti, di, ai, pi;
        std::cin >> si >> ti >> di >> ai >> pi;
        si--; ti--;
        rqs[i] = Request(si, ti, di, ai, pi);
        timeline[si].insert(di);
        timeline[ti].insert(ai);
    }

    std::map<std::pair<int, int>, int> indexMap;   // map the <station, time> pair to graph index 
    int idx = 0; int totalNum = 0;
    for(int i = 0; i < S; ++i){
        for(int t : timeline[i]){
            indexMap[std::make_pair(i, t)] = idx++;
            time[i].push_back(t);
        }
        totalNum += timeline[i].size();
    }
    Graph G(totalNum);
    Vertex src = boost::add_vertex(G);
    Vertex tar = boost::add_vertex(G);

    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
    EdgeWeightMap weightmap = boost::get(boost::edge_weight, G);
    ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
    ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, weightmap, revedgemap);

    // Add edges for each station through the timeline
    for(int i = 0; i < S; ++i){
        Vertex start = indexMap[std::make_pair(i, 0)];
        Vertex end = indexMap[std::make_pair(i, MAXT)];
        eaG.addEdge(src, start, cars[i], 0);
        eaG.addEdge(end, tar, INT_MAX, 0);

        for(int j = 0; j < time[i].size() - 1; ++j){
            int t0 = time[i][j], t1 = time[i][j + 1];
            Vertex u = indexMap[std::make_pair(i, t0)]; 
            Vertex v = indexMap[std::make_pair(i, t1)];
            eaG.addEdge(u, v, INT_MAX, (t1 - t0) * MAXP);
        }
    }

    // Add the edges regarding to booking rqs
    for(int i = 0; i < N; ++i){
        Vertex u = indexMap[std::make_pair(rqs[i].s, rqs[i].d)];
        Vertex v = indexMap[std::make_pair(rqs[i].t, rqs[i].a)];
        eaG.addEdge(u, v, 1, (rqs[i].a - rqs[i].d) * MAXP - rqs[i].p);
    }

    // Compute the maximum profit
    boost::successive_shortest_path_nonnegative_weights(G, src, tar);
    int cost = boost::find_flow_cost(G);
    OutEdgeIt e, eend;
    int maxflow = 0;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(src, G), G); e != eend; ++e){
        maxflow += capacitymap[*e] - rescapacitymap[*e];
    }
    int maxProfit = maxflow * MAXP * MAXT - cost;
    std::cout << maxProfit << "\n";
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
