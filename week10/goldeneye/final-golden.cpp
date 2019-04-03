/**
 * Algorithm: ( improved based on new-golden.cpp )
 * (1). Use Union-Find to find the edge with minimal length that connects s to t
 * (2). Use 3 union-find for three sub-tasks 
 * (3). Notice that there are 3 important constraints: squared_distance_source, squared_distance_target,  min_edge_length 
 * The complexity is O((m + n)logn + n * alpha(n))      (each call of nearest_vertex() costs logN )
 */ 
#include <iostream>
#include <vector>
#include <algorithm>

// CGAL includes
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

// Kernel definitions
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

// Triangulation definitions
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

typedef std::pair<Point, int> Jammer;  // <Point, vertex_index>

// Use boost Union Find
#include <boost/pending/disjoint_sets.hpp>
typedef boost::disjoint_sets_with_storage<> Uf;

class myEdge{
    public:
        int u, v;
        K::FT length;
    
        myEdge(int u, int v, K::FT len){
            this->u = u;
            this->v = v;
            this->length = len;
        }
};

bool my_comparator(myEdge e, myEdge f){
    return e.length < f.length;
}

void testcase(){
    int n, m;    // n: # of jammers, m: # of missions
    long p;      // p: initial power consumption
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
    std::vector<myEdge> edgeList;

    Uf ua(n);  // for the first task  
    for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        // compute exact edge length
        Segment s = t.segment(e);
        K::Point_2 p1 = s.source(), p2 = s.target();
        K::FT w = CGAL::squared_distance(p1, p2);

        // Extract the vertex index of the edge
        Vertex_handle handle_u = e->first->vertex((e->second + 1) % 3);
        Vertex_handle handle_v = e->first->vertex((e->second + 2) % 3);
        int u = handle_u->info(), v = handle_v->info();

        // Store edges into edge list (later for sorting)
        edgeList.push_back(myEdge(u, v, w));

        // Update the union find
        if (w <= p)
            ua.union_set(u, v);
    }

    // Sort edges in increasing order w.r.t. edge length
    std::sort(edgeList.begin(), edgeList.end(), my_comparator);
    K::FT max_among_missions = 0;   // for Task B, find the maximum of max_dist among all the missions
    K::FT max_among_satisfied = 0;  // for Task C, find the maximum of max_dist among all the executable missions

    Uf ub(n), uc(n);   // For second and third tasks
    int idx_b = 0, idx_c = 0;

    for(int i = 0; i < m; ++i){
        double xs, ys, xt, yt;
        std::cin >> xs >> ys >> xt >> yt;
        Point si(xs, ys), ti(xt, yt);

        // Find nearest neighbor of s, t on triangulation
        Vertex_handle vs = t.nearest_vertex(si);
        Vertex_handle vt = t.nearest_vertex(ti);
        
        int src = vs->info();
        int tar = vt->info();

        K::FT squared_dist_src = 4 * CGAL::squared_distance(si, vs->point());
        K::FT squared_dist_tar = 4 * CGAL::squared_distance(ti, vt->point());
        K::FT max_dist = std::max(squared_dist_src, squared_dist_tar);

        // Task A:
        bool executable = false;
        if (ua.find_set(src) == ua.find_set(tar)
            && squared_dist_src <= p && squared_dist_tar <= p){
            std::cout << "y";
            executable = true;
        }
        else
            std::cout << "n";

        // Task B:
        while(ub.find_set(src) != ub.find_set(tar)){
            ub.union_set(edgeList[idx_b].u, edgeList[idx_b].v);
            idx_b++;
        }
        K::FT max_dist_b = std::max(max_dist, edgeList[idx_b - 1].length);
        max_among_missions = std::max(max_among_missions, max_dist_b);
        
        // Task C:
        if(executable){
            while(uc.find_set(src) != uc.find_set(tar)){
                uc.union_set(edgeList[idx_c].u, edgeList[idx_c].v);
                idx_c++;
            }
            K::FT max_dist_c = std::max(max_dist, edgeList[idx_c - 1].length);
            max_among_satisfied = std::max(max_among_satisfied, max_dist_c);
        }
    }

    std::cout << "\n";
    std::cout << max_among_missions << "\n";
    std::cout << max_among_satisfied << "\n";
    // Output:
    // (1). a list of "y"/"n" to indicate if it is possible to complete the mission
    // (2). minimal possible p to satisfy all the missions
    // (3). minimal possible p to satisfy the same number of missions
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    int t = 0;
    std::cin >> t;
    for(int i = 0; i < t; ++i){
        testcase();
    }
    return 0;
}

