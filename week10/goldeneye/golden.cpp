/**
 * Algorithm: construct a graph with edge length less than w and check whether s and t are in the same connected component
 * naive solution: 20pts
 */  
#include <iostream>
#include <vector>
#include <algorithm>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/connected_components.hpp>

// Kernel definitions
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// Triangulation definitions
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, K::FT> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

typedef std::pair<Point, int> Jammer;  // <Point, vertex_index>

void testcase(){
    int n, m; // n: # of jammers, m: # of missions
    long p;   // p: initial power consumption
    std::cin >> n >> m >> p;

    std::vector<Jammer> jammers(n);
    for(int i = 0; i < n; ++i){
        double xi, yi;
        std::cin >> xi >> yi;
        jammers[i] = std::make_pair(Point(xi, yi), i); 
    }

    // construct triangulation from jammer points
    Triangulation t;
    t.insert(jammers.begin(), jammers.end());

    // construct a weighted undirected graph based on triangulation edge length
    Graph G(n);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        // compute exact edge length
        Segment s = t.segment(e);
        K::Point_2 p1 = s.source(), p2 = s.target();
        K::FT w = CGAL::squared_distance(p1, p2);

        // Extract the vertex index of the edge
        Vertex_handle handle_u = e->first->vertex((e->second + 1) % 3);
        Vertex_handle handle_v = e->first->vertex((e->second + 2) % 3);
        Vertex u = handle_u->info(), v = handle_v->info();

        // Add edge to graph G
        if(w <= p){
            Edge e; bool success;
            boost::tie(e, success) = boost::add_edge(u, v, G);
            weightmap[e] = w;
        }
    }

    K::FT max_among_missions = std::numeric_limits<K::FT>().min();
    K::FT max_among_satisfied = std::numeric_limits<K::FT>().min();

    for(int i = 0; i < m; ++i){
        double xs, ys, xt, yt;
        std::cin >> xs >> ys >> xt >> yt;
        Point si(xs, ys), ti(xt, yt);

        // find nearest neighbor of s, t on triangulation
        Vertex_handle vs = t.nearest_vertex(si);
        Vertex_handle vt = t.nearest_vertex(ti);
        
        Point ps = vs->point();
        Point pt = vt->point();

        Vertex vertex_s = vs->info();
        Vertex vertex_t = vt->info();

        K::FT squared_dist_from_s = 4 * CGAL::squared_distance(si, ps);
        K::FT squared_dist_from_t = 4 * CGAL::squared_distance(ti, pt);

        // compute the shortest path from vs to vt;
        // and extract the longest path within shortest path 
        K::FT max_dist = std::numeric_limits<K::FT>().min();
        max_dist = std::max(squared_dist_from_s, squared_dist_from_t);

        std::vector<int> componentmap(n);
        int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
        if(componentmap[vertex_s] == componentmap[vertex_t]){
            if(max_dist <= p){
                std::cout << "y";
            }
            else{
                std::cout << "n";
            }
        }
        else{
            std::cout << "n";
        }
    }

    std::cout << "\n";
    std::cout << 4 * p << "\n";
    std::cout << p << "\n";
    // Output:
    // (1). a list of "y"/"n" to indicate if it is possible to complete the mission
    // (2). minimal possible p to satisfy 
    // (3). minimal possible p to satify the same number of missions
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
