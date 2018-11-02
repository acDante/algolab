/* Method
1. Compute minimal spanning tree for each network
2. Construct the final graph G as the union of all the MSTs (assign the edge weight as the minimal value)
3. Find shortest path by Dijkstra
*/
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges

void testcase(){
    int N, E, S, start, end;
    std::cin >> N >> E >> S >> start >> end;

    std::vector<Graph> nets(S, Graph(N));
    std::vector<WeightMap> weightmaps(S);
    std::vector<int> hives(S);

    for(int i = 0; i < S; ++i){
        weightmaps[i] = boost::get(boost::edge_weight, nets[i]);
    }
   
    for(int i = 0; i < E; ++i){
        int u, v;
        std::cin >> u >> v;
        bool success; Edge e;
    
        for(int s = 0; s < S; ++s){
            int wi; std::cin >> wi;
            boost::tie(e, success) = boost::add_edge(u, v, nets[s]);
            weightmaps[s][e] = wi;
        }
    }
    
    for(int i = 0; i < S; ++i){
        std::cin >> hives[i];
    }
    
    Graph G(N);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    for(int s = 0; s < S; ++s){
        // find private networks with root vertex as hive (Prim MST)
        std::vector<Vertex> primpredmap(N);
        boost::prim_minimum_spanning_tree(nets[s], boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, nets[s])),	// Prim from user-defined start,
			boost::root_vertex(hives[s]));

        for(int i = 0; i < N; ++i){
            if(primpredmap[i] != i){
                Edge e; bool success;
                boost::tie(e, success) = boost::edge(i, primpredmap[i], nets[s]);
                int weight = weightmaps[s][e];
                boost::tie(e, success) = boost::edge(i, primpredmap[i], G);
                if(success){
                    weightmap[e] = std::min(weightmap[e], weight);
                }
                else{
                    boost::tie(e, success) = boost::add_edge(i, primpredmap[i], G);                    
                    weightmap[e] = weight;
                }
            }
        }
    }

    // use Dijkstra to find shortest path
    std::vector<Vertex> predmap(N);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(N);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	boost::dijkstra_shortest_paths(G, start, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter
    
    std::cout << distmap[end] << std::endl;
}

int main(){
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
