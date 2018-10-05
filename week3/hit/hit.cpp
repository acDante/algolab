#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

void testcase(int n){
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    Point p1(x, y), p2(a, b);
    Ray ray(p1, p2);
    bool hitted = false;
    for(int i = 0; i < n; ++i){
        long r, s, t, u;
        std::cin >> r >> s >> t >> u;
        if(!hitted){
            Point s1(r, s), s2(t, u);
            Segment seg(s1, s2);
            if(CGAL::do_intersect(ray, seg)){
                hitted = true;
            }
        }
    }
    if(hitted)
        std::cout << "yes";
    else
        std::cout << "no";
    std::cout << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int n; std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
