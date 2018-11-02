/* Method
1. Compute the size of strongly connected components (SCC) with teleportable vertices
2. Add a new "center" vertex for each SCC and connect the teleportable vertices within SCC to this new vertex
3. Add a super source vertex, connecting to all of the warehourse vertices, run dijkstra from it
*/

#include <iostream>
#include <vector>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/graph_utility.hpp>

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

void print_components(std::vector<std::vector<int> > component_vertices);

void testcase(){
     int n, m, k, T;
     std::cin >> n >> m >> k >> T;
     std::vector<int> telepoints;
     telepoints.reserve(T);
     for(int i = 0; i < T; ++i){
         int ti = 0;
         std::cin >> ti;
         telepoints.push_back(ti);
     }
     Graph G(n);
     WeightMap weightmap = boost::get(boost::edge_weight, G);
     for(int i = 0; i < m; ++i){
         int u, v, c;
         std::cin >> u >> v >> c;
         Edge e; bool success;
         boost::tie(e, success) = boost::add_edge(u, v , G);
         weightmap[e] = c;
     }
     // for debugging
     // boost::print_graph(G);

     // compute strongly connected components
    std::vector<int> component(n), discover_time(n);
    std::vector<boost::default_color_type> color(n);
    std::vector<Vertex> root(n);
    int component_num = strong_components(G, boost::make_iterator_property_map(component.begin(), boost::get(boost::vertex_index, G)), 
                              boost::root_map(boost::make_iterator_property_map(root.begin(), boost::get(boost::vertex_index, G))).
                              color_map(boost::make_iterator_property_map(color.begin(), boost::get(boost::vertex_index, G))).
                              discover_time_map(boost::make_iterator_property_map(discover_time.begin(), boost::get(boost::vertex_index, G))));
    
    std::vector<int> size(component_num, 0);  // store the size of each SCC (only consider telepotation vertices)
    std::vector<std::vector<int> > component_vertices(component_num, std::vector<int>());  // store the vertices in each SCC (only consider telepotation vertices)
    for(int ti: telepoints){
        size[component[ti]]++;
        component_vertices[component[ti]].push_back(ti);
    }

    int numAdded = 0;   // number of added vertices
    for(int i = 0; i < component_num; ++i){
        if(size[i] > 1){
            Vertex center = boost::add_vertex(G);   //  Add a center vertex, and connect each vertex in this SCC to this center vertex
            for(int vi : component_vertices[i]){
                Edge e, re; bool success;
                boost::tie(e, success) = boost::add_edge(vi, center, G);
                weightmap[e] = 0;
                boost::tie(re, success) = boost::add_edge(center, vi, G);
                weightmap[re] = size[i] - 1;
            }
            numAdded++;
        }
    }

    // print_components(component_vertices);

    // Add a new source vertex, pointing to all the warehouse vertices
    Vertex src = boost::add_vertex(G); numAdded++;
    for(int i = 0; i < k; ++i){
        Edge e; bool success;
        boost::tie(e, success) = boost::add_edge(src, i, G);
        weightmap[e] = 0;
    }

    // Run Dijkstra to find shortest path from the added source 
    std::vector<Vertex> predmap(n + numAdded);	// We will use this vector as an Exterior Property Map: Vertex -> Dijkstra Predecessor
	std::vector<int> distmap(n + numAdded);		// We will use this vector as an Exterior Property Map: Vertex -> Distance to source
	boost::dijkstra_shortest_paths(G, src, // We MUST provide at least one of the two maps
		boost::predecessor_map(boost::make_iterator_property_map(predmap.begin(), boost::get(boost::vertex_index, G))).	// predecessor map as Named Parameter
		distance_map(boost::make_iterator_property_map(distmap.begin(), boost::get(boost::vertex_index, G))));	// distance map as Named Parameter

    // std::cout << distmap[n - 1] << std::endl;
    if(distmap[n - 1] > 1000000){
        std::cout << "no" << "\n";
    }
    else{
        std::cout << distmap[n - 1] << "\n";
        // std::cout << predmap[n - 1] << "-"  << "\n";
    }
}

void print_components(std::vector<std::vector<int> > component_vertices){
    for(int i = 0; i < component_vertices.size(); ++i){
        std::cout << i << "-th component:" <<std::endl;
        for(int vi: component_vertices[i]){
            std::cout << vi << " ";
        }
        std::cout << std::endl;
    }
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