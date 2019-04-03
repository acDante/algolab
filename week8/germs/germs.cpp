/**
 * Attention: Remember to use "&" to update the vertex info !!!
 */  
#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Triangulation_vertex_base_with_info_2<double, IK>     Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<IK, Tds>                     Triangulation;
typedef IK::Point_2 Point;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;

EK::FT dist_to_hrs(const EK::FT& dist){
    return CGAL::sqrt((CGAL::sqrt(dist) - 1) / 2);
}

double ceil_to_double(const EK::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

void testcase(int n){
    double l, b, r, t;
    std::cin >> l >> b >> r >> t;
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }
    
    Triangulation trg;
    trg.insert(pts.begin(), pts.end());

    // Find the min distance between point and rectangle
    for(auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v){
        IK::Point_2 p = v->point();
        IK::FT min_dist_x = std::min(p.x() - l, r - p.x());
        IK::FT min_dist_y = std::min(p.y() - b, t - p.y());
        IK::FT min_dist = std::min(min_dist_x, min_dist_y);
        IK::FT & info = v->info();
        info = 4 * min_dist * min_dist;
    }

    // Find the squared distance between nearest neighbor
    for(auto e = trg.finite_edges_begin(); e != trg.finite_edges_end(); ++e){
        Triangulation::Vertex_handle u = e->first->vertex((e->second + 1) % 3);
        Triangulation::Vertex_handle v = e->first->vertex((e->second + 2) % 3);
        IK::Point_2 p1 = u->point(), p2 = v->point();
        IK::FT & u_info = u->info(); IK::FT & v_info = v->info();
        IK::FT min_dist = CGAL::squared_distance(p1, p2);
        
        u_info = std::min(u_info, min_dist);
        v_info = std::min(v_info, min_dist);
    }

    std::vector<IK::FT> dist;
    dist.reserve(n);
    for(auto v = trg.finite_vertices_begin(); v != trg.finite_vertices_end(); ++v){
        dist.push_back(v->info());
    }
    std::sort(dist.begin(), dist.end());

    EK::FT first = dist_to_hrs(EK::FT(dist[0]));
    EK::FT median = dist_to_hrs(EK::FT(dist[n / 2]));
    EK::FT last = dist_to_hrs(EK::FT(dist[n - 1]));

    std::cout << ceil_to_double(first) << " "
              << ceil_to_double(median) << " "
              << ceil_to_double(last) << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n = 0;   // # of bacterias
    std::cin >> n;
    while(n){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
