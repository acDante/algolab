// Naive solution, a bit slower than the optimal solution :)
// STL includes 
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>
#include <climits>

// Delaunay includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>               Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                 Delaunay;
typedef Delaunay::Point                                        Point;
typedef Delaunay::Edge_iterator  Edge_iterator;
typedef Delaunay::Vertex_iterator Vertex_iterator;
typedef Delaunay::Vertex_handle  Vertex_handle;
typedef Delaunay::Edge_circulator Edge_circulator;

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

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

// Check if a 2-coloring is feasible
bool isFeasible(Delaunay & trg, double r){
    for(auto e = trg.finite_edges_begin(); e != trg.finite_edges_end(); e++){
        if(trg.segment(*e).squared_length() <= r * r)
            return false;
    }
    return true;
}

// Check if a connected component (represented by "start") is 2-colorable (or bipartite)
bool isBipartite(Graph& G, int n, int start, std::vector<std::pair<Point, int> > & stations, double r){
    std::vector<bool> color(n);
    std::vector<bool> visited(n, false);
    std::queue<int> Q;
    
    Q.push(start);
    visited[start] = true;
    color[start] = 0;

    Delaunay trg1, trg2;
    trg1.insert(stations[start].first);
    while(!Q.empty()){
        int u = Q.front();
        Q.pop();
        OutEdgeIt oebeg, oeend;
        for(boost::tie(oebeg, oeend) = boost::out_edges(u, G); oebeg != oeend; ++oebeg){
            Vertex v = boost::target(*oebeg, G);
            if(!visited[v]){
                color[v] = 1 - color[u];
                visited[v] = true;
                Q.push(v);

                if(color[v] == 0)
                    trg1.insert(stations[v].first);
                else
                    trg2.insert(stations[v].first);
            }
            else{
                if(color[u] == color[v])
                    return false;
            }
        }   
    }
    /* Attention: the 2-coloring computed above is not always feasible,
    *  Need to construct 2 triangulations based on color and check if there exists an edge <= r
    */
    return isFeasible(trg1, r) && isFeasible(trg2, r); 
}

void testcase(){
    int n, m; double r;    // n: # of stations, m: # of clues, r: range
    std::cin >> n >> m >> r;
    Graph G(n);
    std::vector<std::pair<Point, int> > stations(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        stations[i] = std::make_pair(Point(x, y), i);
    }

    // Construct EMST graph based on delaunay edges with distance <= r
    Delaunay DT;
    DT.insert(stations.begin(), stations.end());
    // for(auto v = DT.finite_vertices_begin(); v != DT.finite_vertices_end(); ++v){
    //     Edge_circulator c = DT.incident_edges(*v);
    //     do{
    //         ;
    //     } while(++c != DT.incident_edges(*v));
    // }

    for(Edge_iterator e = DT.finite_edges_begin(); e!= DT.finite_edges_end(); ++e){
        Vertex_handle u = e->first->vertex((e->second + 1) % 3);
        Vertex_handle v = e->first->vertex((e->second + 2) % 3);
        if(CGAL::squared_distance(u->point(), v->point()) <= r * r){
            Edge e; bool success;
            boost::tie(e, success) = boost::add_edge(u->info(), v->info(), G);
        }
    }

    // Compute connected component map of G and store representative vertex for each component
    std::vector<int> componentmap(n);
    int ncc = boost::connected_components(G, boost::make_iterator_property_map(componentmap.begin(), boost::get(boost::vertex_index, G)));
    std::vector<int> starts;
    std::vector<bool> visited(ncc, false);
    for(int i = 0; i < n; ++i){
        if(!visited[componentmap[i]]){
            visited[componentmap[i]] = true;
            starts.push_back(i);
        }
    }
    assert(starts.size() == ncc);
    bool is_bipartite = true;
    for(int i = 0; i < starts.size(); ++i){
        if(!isBipartite(G, n, starts[i], stations, r)){
            is_bipartite = false;
            break;
        }
    }

    /* Check if there exists feasible connection between a and b:
    * (1). dist(a, t0) <= r
    * (2). dist(b, tk) <= r
    * (3). t0 and tk are in the same connected component
    */  
    for(int i = 0; i < m; ++i){
        double xa, ya, xb, yb;
        std::cin >> xa >> ya >> xb >> yb;
        if(!is_bipartite){
            std::cout << "n";
            continue;
        }
        Point pa = Point(xa, ya), pb = Point(xb, yb);
        if(CGAL::squared_distance(pa, pb) <= r * r){
            std::cout << "y";
            continue;
        }
        Vertex_handle va = DT.nearest_vertex(pa);
        Vertex_handle vb = DT.nearest_vertex(pb);
        if(CGAL::squared_distance(va->point(), pa) <= r * r 
            && CGAL::squared_distance(vb->point(), pb) <= r * r 
            && componentmap[va->info()] == componentmap[vb->info()])
            std::cout << "y";
        else
            std::cout << "n";
    }

    std::cout << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t = 0;
    std::cin >> t;
    for (int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}
