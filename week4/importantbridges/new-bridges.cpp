/* Method
Find all the bi-connected compoents with size 1
*/
#include <iostream>
#include <vector>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_index_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::property_map<Graph, boost::edge_index_t>::type  ComponentMap;

// namespace boost
// {
//   struct edge_component_t
//   {
//     enum
//     { num = 555 };
//     typedef edge_property_tag kind;
//   }
//   edge_component;
// }

void testcase(){
    int n, m;
    std::cin >> n >> m;
    Graph G(n), Gc(n);  // create a copy of Graph G
    for(int i = 0; i < m; ++i){
        int u, v;
        std::cin >> u >> v;
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(u, v, G);
    }
    
    ComponentMap componentMap = boost::get(boost::edge_index, G);
    int ncc = boost::biconnected_components(G, componentMap);
    std::vector<int> component_size(ncc);
    EdgeIt ebeg, eend;
    for(boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ebeg++){
        component_size[componentMap[*ebeg]]++;
    }

    int num_bridges = 0;
    std::set<std::pair<int, int> > vertices;  // to store the vertcies of bridge edges
    for(boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ebeg++){
        if(component_size[componentMap[*ebeg]] == 1){
            num_bridges ++;
            Vertex u = source(*ebeg, G);
            Vertex v = target(*ebeg, G);
            if(u > v)
                std::swap(u, v);
            vertices.insert(std::make_pair(u, v));
        }
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
