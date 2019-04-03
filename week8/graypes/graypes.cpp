/**
 * Algorithm: find th edge wiht minimal length
 */
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;

typedef IK::Point_2 Point;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle Vertex_handle;
typedef Triangulation::Edge_iterator Edge_iterator;

double ceil_to_double(const EK::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(int n){
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        double x, y;
        std::cin >> x >> y;
        pts[i] = Point(x, y);
    }
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    IK::FT min_dist_squared = std::numeric_limits<IK::FT>().max();
    for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
        min_dist_squared = std::min(t.segment(e).squared_length(), 
                                    min_dist_squared);
    }
    double min_dist = CGAL::sqrt(min_dist_squared);
    std::cout << std::ceil(min_dist * 50) << "\n";
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::cout << std::fixed << std::setprecision(0);
    while(n){
        testcase(n);
        std::cin >> n;
    }
}
