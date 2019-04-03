// Delaunay + Connected Component + Binary Search
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cassert>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graph_utility.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>         Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Triangulation;
typedef Triangulation::Edge_iterator                                Edge_iterator;
typedef Triangulation::Vertex_handle                                Vertex_handle;
typedef K::Point_2 Point;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, int> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	

// Find the size of maximal connected components in [pk, ..., pn-1]
int findMaxComponent(int k, int n, double r, std::vector<std::pair<Point, int> >& pts){
    Triangulation t;
    t.insert(pts.begin() + k, pts.end());

    Graph G(n - k);
    for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        if(t.segment(e).squared_length() <= r * r){
            Vertex_handle u = e->first->vertex((e->second + 1) % 3);
            Vertex_handle v = e->first->vertex((e->second + 2) % 3);
            Edge e; bool success;
            boost::tie(e, success) = boost::add_edge(u->info() - k, v->info() - k, G);
        }
    }
    // boost::print_graph(G);
    std::vector<int> componentmap(n - k);
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
    std::vector<int> componentsize(ncc);
    int max_size = 0;    // size of max connected component
    for(int i = 0; i < n - k; ++i){
        ++componentsize[componentmap[i]];
    }
    for(int i = 0; i < ncc; ++i){
        max_size = std::max(max_size, componentsize[i]);
    }
    return max_size;
}

void testcase(){
    int n; double r;
    std::cin >> n >> r;
    std::vector<std::pair<Point, int> > pts(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        pts[i] = std::make_pair(Point(x, y), i);
    }

    // Find the largest k such that max component size >= k by binary search
    int low = 0, high = n / 2;
    int res = 0;
    while(low <= high){
        int k = (low + high) / 2;
        int max_size = findMaxComponent(k, n, r, pts);
        // std::cout << k << " " << max_size  << "\n";
        if(max_size >= k){
            res = k;
            low = k + 1;
        }
        else
            high = k - 1;
    }
    std::cout << res << "\n";
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
