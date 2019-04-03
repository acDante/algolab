// Find second-best MST, use BFS to pre-compute the max edge weight
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <queue>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>

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
typedef std::vector<std::vector<int> > Table;

// Compute the maximal MST edge from a source vertex to all the other vertices by BFS
void findMaxWeight(int src, int n, Graph& mstG, Table& weight, Table& maxWeight ){
    std::vector<bool> visited(n, false);
    std::queue<int> Q;
    Q.push(src);
    visited[src] = true;
    maxWeight[src][src] = 0;
    while(!Q.empty()){
        int u = Q.front();
        Q.pop();
        OutEdgeIt oebeg, oeend;
        for (boost::tie(oebeg, oeend) = boost::out_edges(u, mstG); oebeg != oeend; ++oebeg){
            Vertex v = boost::target(*oebeg, mstG);
            if(!visited[v]){
                Q.push(v);
                visited[v] = true;
                maxWeight[src][v] = std::max(maxWeight[src][u], weight[u][v]);
            }
        }
    }
}

void testcase(){
    int n, start; 
    std::cin >> n >> start;

    // Find MST by Prim's algorithm
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    Table weight(n, std::vector<int>(n, 0));

    for(int i = 0; i < n - 1; i++){
        for(int j = i + 1; j < n; j++){
            int w; std::cin >> w;
            Edge e; bool success;
            boost::tie(e, success) = boost::add_edge(i, j, G);
            weightmap[e] = w;
            weight[i][j] = w; weight[j][i] = w;
        }
    }

    std::vector<Vertex> primpredmap(n);
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
		boost::root_vertex(start - 1));
    
    // Construct MST graph and compute MST total weight
    Graph mstG(n);   
    long totalweight = 0;
    for(int i = 0; i < n; ++i){
        if(primpredmap[i] != i){
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
            totalweight += weightmap[e];
            boost::tie(e, success) = boost::add_edge(i, primpredmap[i], mstG);
        }
    }

    // The idea is: Try to append each non-MST edge to MST graph, and delete MST edge with maximal weight in the formed cycle.
    // Precompute the maximal MST edge in the path u->v to save time  (i.e. maxWeight[u][v]) 
    // Find the minimum of { MST weight + weight[u][v] - maxWeight[u][v] }
    int minDiff = INT_MAX;
    Table maxWeight(n, std::vector<int>(n, -1));
    EdgeIt ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg){
        Vertex u = boost::source(*ebeg, G), v = boost::target(*ebeg, G);
        if(primpredmap[u] == v || primpredmap[v] == u)   // disregard MST edges
            continue;
        if(maxWeight[u][v] == -1)
            findMaxWeight(u, n, mstG, weight, maxWeight);
        
        minDiff = std::min(minDiff, weight[u][v] - maxWeight[u][v]);
     }
    std::cout << totalweight + minDiff << "\n";
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
