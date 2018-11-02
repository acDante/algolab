#include <iostream>
#include <iomanip>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
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
    K::FT r = CGAL::sqrt(c.squared_radius());
    std::cout << std::fixed << std::setprecision(0);
    std::cout << ceil_to_double(r) << std::endl;
}

int main(){
    int n;
    std::cin >> n;
    while(n != 0){
        testcase(n);
        std::cin >> n;
    }
    return 0;
}
