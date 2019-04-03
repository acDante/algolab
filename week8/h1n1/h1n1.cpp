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
#include <boost/graph/kruskal_min_spanning_tree.hpp>

// Delaunay definitions with info 
typedef std::pair<int, double> PID;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<PID,K> Fb;
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

double DOUBLE_MAX = std::numeric_limits<double>().max();
// Find the length of bottleneck edge from infinite face to each face by BFS
void findBottleneck(Graph& G, Triangulation& t, std::vector<Vertex>& predmap, 
                    std::map<int, All_faces_iterator>& faceMap){
    WeightMap weightmap = boost::get(boost::edge_weight, G);
    std::queue<int> Q;
    std::vector<bool> visited(boost::num_vertices(G), false);
    Q.push(0); visited[0] = true;
    faceMap[0]->info().second = DOUBLE_MAX;
    while(!Q.empty()){
        int u = Q.front(); 
        Q.pop();
        OutEdgeIt obeg, oend;
        for(boost::tie(obeg, oend) = boost::out_edges(u, G); obeg != oend; ++obeg){
            Vertex v = boost::target(*obeg, G);
            if(predmap[u] == v || predmap[v] == u && !visited[v]){
                visited[v] = true;
                faceMap[v]->info().second = std::min(faceMap[u]->info().second, 
                                                     DOUBLE_MAX - weightmap[*obeg]);
                Q.push(v);
            }
        }
    }
}

void testcase(int n){
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }

    int m; std::cin >> m;
    std::vector<std::pair<Point, double> > queries(m);

    for(int i = 0; i < m; ++i){
        double x, y, d;
        std::cin >> x >> y >> d;
        queries[i].first = Point(x, y); queries[i].second = d;
    }

    Triangulation DT;
    DT.insert(pts.begin(), pts.end());

    int face_idx = 1;
    std::map<int, All_faces_iterator> faceMap;  // Map graph index to face handle
    for(auto f = DT.all_faces_begin(); f != DT.all_faces_end(); ++f){
        PID & info = f->info();
        if(DT.is_infinite(f))
            info.first = 0;
        else{
            info.first = face_idx++;
        }
        faceMap.insert(std::make_pair(info.first, f));
    }

    int num_faces = face_idx;
    Graph G(num_faces);
    WeightMap weightmap = boost::get(boost::edge_weight, G);

    // Construct graph based on Triangulation faces
    for(auto f = DT.all_faces_begin(); f != DT.all_faces_end(); ++f){
        bool current_is_infinite = DT.is_infinite(f);
        Vertex u = f->info().first;
        for(int i = 0; i < 3; ++i){
            if(DT.is_infinite(f->neighbor(i)))   // only consider finite neighbors, to avoid adding repeated edges
                continue;
            Vertex v = f->neighbor(i)->info().first;
            Vertex_handle v1 = f->vertex((i + 1) % 3), v2 = f->vertex((i + 2) % 3);
            double l2 = CGAL::squared_distance(v1->point(), v2->point());
            Edge e; bool success;
            boost::tie(e, success) = boost::edge(u, v, G);
            if(!success){
                boost::tie(e, success) = boost::add_edge(u, v, G);
                weightmap[e] = DOUBLE_MAX - l2;
            }
            else if(current_is_infinite)
                weightmap[e] = std::min(weightmap[e], DOUBLE_MAX - l2);
        }
    }

    // Construct maximal spanning tree graph
    int INF_FACE = 0;
    std::vector<Vertex> primpredmap(boost::num_vertices(G));
    boost::prim_minimum_spanning_tree(G, boost::make_iterator_property_map(primpredmap.begin(), boost::get(boost::vertex_index, G)),	// Prim from user-defined start,
		boost::root_vertex(INF_FACE));
    
    findBottleneck(G, DT, primpredmap, faceMap);
    // Check the bottleneck edge for each query
    for(auto query : queries){
        Point person = query.first; 
        Vertex_handle vh = DT.nearest_vertex(person);
        if(CGAL::squared_distance(vh->point(), person) < query.second){
            std::cout << "n";
            continue;
        }
        Face_handle f = DT.locate(person);
        if(DT.is_infinite(f)){
            std::cout << "y";
            continue;
        }

        double lower_bound = 4 * query.second;
        if(f->info().second >= lower_bound)
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
