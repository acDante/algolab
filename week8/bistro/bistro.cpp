#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef K::Point_2 Point;

void testcase(int n){
    int m;
    std::vector<Point> pts(n);
    for(int i = 0; i < n; ++i){
        int xi, yi;
        std::cin >> xi >> yi;
        pts[i] = Point(xi, yi);
    }

    Triangulation t;
    t.insert(pts.begin(), pts.end());

    std::cin >> m;
    std::vector<Point> new_pts(m);
    for(int i = 0; i < m; ++i){
        int xi, yi;
        std::cin >> xi >> yi;
        Point p(xi, yi);
        Point neighbor = t.nearest_vertex(p)->point();
        std::cout << CGAL::squared_distance(p, neighbor) << "\n";
    }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n;
    std::cin >> n;
    std::cout << std::fixed << std::setprecision(0);
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
}
