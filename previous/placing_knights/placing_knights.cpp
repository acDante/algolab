/*
* Maximum independent set on bipartite graph (consisting of black and white cells)
* Notice that white cells are always constrained by neighboring black cells and vice versa
* Be careful with holes (not add edges in bipartite graph)
*/
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

bool isValid(int n, int row, int col){
    return (row >= 0 && row < n) && (col >= 0 && col < n);
}

void testcase(){
    int n;
    std::cin >> n;
    int num_holes = 0;
    std::vector<std::vector<bool> > chessboard(n, std::vector<bool>(n, 0));
    std::vector<std::vector<int> > vid(n, std::vector<int>(n, 0));

    Graph G(n * n);
	EdgeCapacityMap capacitymap = boost::get(boost::edge_capacity, G);
	ReverseEdgeMap revedgemap = boost::get(boost::edge_reverse, G);
	ResidualCapacityMap rescapacitymap = boost::get(boost::edge_residual_capacity, G);
    EdgeAdder eaG(G, capacitymap, revedgemap);
    int src = boost::add_vertex(G);
    int tar = boost::add_vertex(G);
    
    for(int row = 0; row < n; ++row){
        for(int col = 0; col < n; ++col){
            int c = 0; std::cin >> c;
            chessboard[row][col] = c;
            vid[row][col] = row * n + col;
            if(chessboard[row][col]){                
                if((row + col) % 2 == 0)    // white cells
                    eaG.addEdge(src, vid[row][col], 1);
                else    // black cells
                    eaG.addEdge(vid[row][col], tar, 1);
            }
            else
                num_holes++;
        }
    }
    std::vector<int> dx{-1, -1, 1, 1, -2, -2, 2, 2};
    std::vector<int> dy{-2, 2, -2, 2, -1, 1, -1, 1};

    for(int row = 0; row < n; ++row){
        for(int col = 0; col < n; ++col){
            // Only add edges for valid grids
            // always connect edges from white grids to black grids
            // whenever grids have dependence
            if(chessboard[row][col] && (row + col) % 2 == 0){
                for(int i = 0; i < dx.size(); ++i){
                    int nrow = row + dx[i], ncol = col + dy[i];
                    if (isValid(n, nrow, ncol)){
                        eaG.addEdge(vid[row][col], vid[nrow][ncol], 1);
                    }
                }
            }
        }
    }
    // compute size of minimal vertex cover
    // size of maximum independent set = total valid places - size of minVC
    long flow = boost::push_relabel_max_flow(G, src, tar);
    int validPlaces = n * n - num_holes;
    long maxIS = validPlaces - flow;
    std::cout << maxIS << "\n";
}

int main(){
    int T = 0;
    std::cin >> T;
    for(int i = 0; i < T; ++i){
        testcase();
    }
    return 0;
}
