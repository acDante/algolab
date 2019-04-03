/** Algorithm: max flow (supply-demand problem)
 *  Each position of the newspaper can provide a supply of the letter on its front or back side,
 *  The note needs a certain demand for each letter,
 *  Check whether the maxflow equals to the note size
 */
// STL includes
#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>

// BGL Graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef	boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> Traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
	boost::property<boost::edge_capacity_t, long,
		boost::property<boost::edge_residual_capacity_t, long,
			boost::property<boost::edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
// Interior Property Maps
typedef	boost::property_map<Graph, boost::edge_capacity_t>::type		EdgeCapacityMap;
typedef	boost::property_map<Graph, boost::edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	boost::property_map<Graph, boost::edge_reverse_t>::type		ReverseEdgeMap;
typedef	boost::graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	boost::graph_traits<Graph>::edge_descriptor			Edge;
typedef	boost::graph_traits<Graph>::edge_iterator			EdgeIt;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class EdgeAdder {
	Graph &G;
	EdgeCapacityMap	&capacitymap;
	ReverseEdgeMap	&revedgemap;

public:
	// to initialize the Object
	EdgeAdder(Graph & G, EdgeCapacityMap &capacitymap, ReverseEdgeMap &revedgemap):
		G(G), capacitymap(capacitymap), revedgemap(revedgemap){}

	// to use the Function (add an edge)
	void addEdge(int from, int to, long capacity) {
		Edge e, rev_e;
		bool success;
		boost::tie(e, success) = boost::add_edge(from, to, G);
		boost::tie(rev_e, success) = boost::add_edge(to, from, G);
		capacitymap[e] = capacity;
		capacitymap[rev_e] = 0; // reverse edge has no capacity!
		revedgemap[e] = rev_e;
		revedgemap[rev_e] = e;
	}
};

int vertex_index(char wf, char wb){   // compute vertex index for pair(wf, wb), wf = letter on the front, wb = letter on the back
    int index = (wf - 'A') * 26 + (wb - 'A');
    return index;
}

void testcase(){
    const int NUM = 26;
    int h, w;   // h: # of lines, w: # of letters per line
    std::cin >> h >> w;
    std::string note;
    std::cin >> note;
    std::vector<std::vector<char> > front(h, std::vector<char>(w));
    std::vector<std::vector<char> > back(h, std::vector<char>(w));

    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            std::cin >> front[i][j];
        }
    }

    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            std::cin >> back[i][j];
        }
    }

    // count the number of each letter in the note
    std::vector<int> count(NUM, 0);
    for(int i = 0; i < note.size(); ++i){
        count[note[i] - 'A'] ++;
    }

    // count the number of each front-back letter pair (wf, wb)  -> 26 * 26 possible pairs in total
    std::vector<int> pair_count(NUM * NUM, 0);
    for(int i = 0; i < h; ++i){
        for(int j = 0; j < w; ++j){
            int idx = vertex_index(front[i][j], back[i][w - j -1]);
            pair_count[idx] ++;
        }
    }

    Graph G(NUM * NUM + NUM);  // initialize with 26 * 26 + 26 vertices
    EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
	EdgeAdder eaG(G, capacitymap, revedgemap);

    Vertex src = boost::add_vertex(G);
    Vertex sink = boost::add_vertex(G);
    for(int i = 0; i < NUM; ++i){
        eaG.addEdge(NUM * NUM + i, sink, count[i]);   // each letter requires a certain demand
    }

    for(int i = 0; i < NUM * NUM; ++i){     // connect each <front, back> letter pair to the letters it can provide
        eaG.addEdge(src, i, pair_count[i]);
        int front_index = i / 26 + NUM * NUM;
        int back_index = i % 26 + NUM * NUM;
        eaG.addEdge(i, front_index, pair_count[i]);
        eaG.addEdge(i, back_index, pair_count[i]);
    }

    long flow = boost::push_relabel_max_flow(G, src, sink);
    if(flow == note.size()){
        std::cout << "Yes" << "\n";
    }
    else{
        std::cout << "No" << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
}