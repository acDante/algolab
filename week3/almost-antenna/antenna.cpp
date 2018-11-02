/* Method
1. Compute minimal enclosing circle
2. Remove one of the points on the boundary of min circle, and compute min circle with remaining points
*/
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>     Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

double ceil_to_double(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

void testcase(int n){
    std::vector<K::Point_2> pts;
    pts.reserve(n);
    for(int i = 0; i < n; ++i){
        long xi, yi;
        std::cin >> xi >> yi;
        pts.push_back(K::Point_2(xi, yi));
    }

    Min_circle mc(pts.begin(), pts.end(), true);
    Traits::Circle c = mc.circle();
    K::FT minSquared = c.squared_radius();

    for(int i = 0; i < mc.number_of_support_points(); ++i){
        // 去掉某一个support point 然后用余下的点算min circle
        K::Point_2 sp = mc.support_point(i);
        std::vector<K::Point_2>::iterator itr;
        itr = std::find(pts.begin(), pts.end(), sp);
        if(itr != pts.end()){
            std::swap(pts[0], *itr);
            Min_circle new_mc(pts.begin() + 1, pts.end(), true);
            Traits::Circle new_c = new_mc.circle();
            K::FT squared = new_c.squared_radius();
            if(squared < minSquared){
                minSquared = squared;
            }
            std::swap(pts[0], *itr);
        }
    }
    std::cout << ceil_to_double(CGAL::sqrt(minSquared)) << std::endl;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cout << std::fixed << std::setprecision(0);
    int n;
    std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}

