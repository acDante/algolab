// Maximum Spanning Tree + BFS for precomputing the min edge length
// std::numeric_limits<double>().max() is buggy sometimes.. Use INT64_MAX instead
// STL includes
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <queue>

// Delaunay includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/graph_utility.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds> Triangulation;

typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::All_faces_iterator All_faces_iterator;
typedef K::Point_2 Point;

// BGL Graph definitions
// =====================
// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.
		boost::no_property,				// interior properties of vertices	
		boost::property<boost::edge_weight_t, double> 		// interior properties of edges
		>					Graph;
typedef boost::graph_traits<Graph>::edge_descriptor		Edge;		// Edge Descriptor: an object that represents a single edge.
typedef boost::graph_traits<Graph>::vertex_descriptor		Vertex;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<Graph>::edge_iterator		EdgeIt;		// to iterate over all edges
typedef boost::graph_traits<Graph>::out_edge_iterator		OutEdgeIt;	// to iterate over all outgoing edges of a vertex
typedef boost::property_map<Graph, boost::edge_weight_t>::type	WeightMap;	// property map to access the interior property edge_weight_t

// double MAX_LENGTH = std::numeric_limits<double>().max();
// const double MAX_LENGTH = 32000000000000000.0;
const double MAX_LENGTH = INT64_MAX; 
void testcase(int n){
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }
    Triangulation DT;
    DT.insert(pts.begin(), pts.end());

    int num_face = 1;
    for(auto f = DT.all_faces_begin(); f != DT.all_faces_end(); f++){
        if(DT.is_infinite(f))
            f->info() = 0;
        else{
            f->info() = num_face++;
        }
    }

    Graph G(num_face);
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    
    // Construct graph based on Triangulation faces
    for(auto f = DT.all_faces_begin(); f != DT.all_faces_end(); ++f){
        Vertex u = f->info();
        for(int i = 0; i < 3; ++i){
            auto nb = f->neighbor(i);
            if(DT.is_infinite(nb))   // only consider finite neighbors, to avoid adding repeated edges
                continue;
            Vertex v = nb->info();
            double l2 = DT.segment(f, i).squared_length();
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(u, v, G);
            if(!success){
                boost::tie(e, success) = boost::add_edge(u, v, G);
                weightmap[e] = MAX_LENGTH - l2;
             }
            else if(DT.is_infinite(f)){
                weightmap[e] = std::min(weightmap[e], MAX_LENGTH - l2);
            }
        }
    }
    // boost::print_graph(G);
    // Compute Maximum Spanning Tree of Face Graph
    int INF_FACE = 0;
    std::vector<Vertex> primpredmap(boost::num_vertices(G));
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
		boost::root_vertex(INF_FACE));
    
    // Find bottleneck edge length for each face by BFS on MST edges
    std::vector<double> minEdge(num_face, MAX_LENGTH);
    std::queue<int> q;
    q.push(INF_FACE); 
    while(!q.empty()){
        int u = q.front();
        q.pop();
        OutEdgeIt oebeg, oeend;
        for(boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; ++oebeg){
            Vertex v = boost::target(*oebeg, G);
            if(primpredmap[v] == u){
                minEdge[v] = std::min(minEdge[u], MAX_LENGTH - weightmap[*oebeg]);
                q.push(v);
            }
        }
    }
    int m; std::cin >> m;
    for(int i = 0; i < m; ++i){
        double x, y, d;
        std::cin >> x >> y >> d;
        Point p(x, y);
        Vertex_handle vh = DT.nearest_vertex(p);
        if(CGAL::squared_distance(vh->point(), p) < d){
            std::cout << "n";
            continue;
        }
        Face_handle f = DT.locate(p);
        if(DT.is_infinite(f)){
            std::cout << "y";
            continue;
        }
        if(minEdge[f->info()] >= 4 * d)
            std::cout << "y";
        else
            std::cout << "n";
    }
    std::cout << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n = 0;
    std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
}
