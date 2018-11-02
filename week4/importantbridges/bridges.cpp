// Remove each edge and check whether the number of connected components increase O(E * (V + E)).. not fast enough
#include <iostream>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges

void testcase(){
    int n, m;
    std::cin >> n >> m;
    Graph G(n), Gc(n);  // create a copy of Graph G
    for(int i = 0; i < m; ++i){
        int u, v;
        std::cin >> u >> v;
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(u, v, G);
        boost::tie(e, success) = boost::add_edge(u, v, Gc);
    }

    std::vector<int> componentmap(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int num_components = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G))); 

    // Remove each edge and check whether the number of connected components increase
    int num_bridges = 0;
    std::set<std::pair<int, int> > vertices;
    EdgeIt ebeg, eend;
    for(boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg){
        Vertex u = source(*ebeg, G);
        Vertex v = target(*ebeg, G);
        boost::remove_edge(u, v, Gc);

        std::vector<int> componentmap(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	    int ncc = boost::connected_components(Gc, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, Gc))); 

        if(ncc > num_components){
            num_bridges++;
            if(u > v) 
                std::swap(u, v);
            vertices.insert(std::make_pair(u, v));
        }

        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(u, v, Gc);
    }

    std::cout << num_bridges << std::endl;
    for(std::pair<int,int> v : vertices){
        std::cout << v.first << " " << v.second << std::endl;
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int c = 0;
    std::cin >> c;
    for(int i = 0; i < c; ++i){
        testcase();
    }
}
